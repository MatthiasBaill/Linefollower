#include "SerialCommand.h"
#include "EEPROMAnything.h"

#define SerialPort Serial1 //Arduino leonarde ETH communiceerd via serial1 poort
#define Baudrate 9600 

#define  BIN2 10 // MotorRightForward (Benable)
#define  BIN1 11 // MotorRightBackward (Bphase)
#define  AIN2 3 // MotorLeftForward(Aenable)
#define  AIN1 9 // MotorLeftForward (Aphase)

SerialCommand sCmd(SerialPort); // SerialCommand object declaration 
bool debug;
bool run;
//bool stop; 
unsigned long Current;
unsigned long previous;
unsigned long calculationTime;
unsigned long Difference; 
unsigned long cycleTime = 2500;
float iTerm = 0;
float lastErr; 

//sensoren
const int sensor[] = { A5, A4, A3, A2, A1, A0}; 

const int led = 8;
const int Drukknop = 2; 


// EEPROM opgeslagen parameteres 
struct param_t
{
  unsigned long cycleTime; 
  int black[6];
  int white[6];
  int power; 
  float diff;
  float kp;  
  float ki; 
  float kd;
} params;

  int normalised[6];
  float debugPosition;
  float output;

void setup()
{
  SerialPort.begin(Baudrate); // set serial baudrate at 9600, serial bleutooth apps werken op een 9600 baudrate

  sCmd.addCommand("set", onSet);
  sCmd.addCommand("debug", onDebug);
  sCmd.addCommand("calibrate", onCalibrate);
  sCmd.addCommand("run", onRun);
  sCmd.addCommand("stop", onStop); 
  sCmd.setDefaultHandler(onUnknownCommand);
  EEPROM_readAnything(0, params);

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH); //Eerste controle licht, ingebouwde led

  pinMode(Drukknop, OUTPUT); //Interupt
  pinMode(BIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(AIN1, OUTPUT);
  
  SerialPort.println("ready");
}

void loop()
{
  sCmd.readSerial(); // listen and process serial commands
  
  unsigned long current = micros();
  if (current - previous >= params.cycleTime)
  {
    previous = current;
  
    for (int i = 0; i < 6; i++)
    {
      normalised[i] = map(analogRead(sensor[i]), params.black[i], params.white[i], 0, 1000); 
    }

    float position = 0; // positie bepalen sensoren 
    int index = 0; // zwartste sensor bepalen

    for(int i = 1; i < 6; i++) if (normalised[i] < normalised[index]) index = i; // nieuwe sensor met hoogste zwartst waarde zoeken 

    if (normalised[index]); 

    if (index == 0) position = -30; 
    else if (index == 5) position = 30;
    else
    {
      int sensorNul = normalised[index];
      int sensorMinEen = normalised[index-1];
      int sensorPlusEen = normalised[index+1];

      float b = sensorPlusEen - sensorMinEen;  // b berekenen 
      b = b / 2; 

      float a = sensorPlusEen - b - sensorNul; 

      position = -b / (2 * a); 
      position += index;
      position -= 2.5;
      position *= 15;      
    }
    debugPosition = position; 
    
    /* normaliseren en interpoleren sensor
    bereken error = setpoint - positie */
    float error = -position;

    /* proportioneel regelaar */
    float output = error * params.kp;


    /* integrerend regelaar */
    iTerm += params.ki * error;
    iTerm = constrain(iTerm, -510, 510);
    output += iTerm;

    /*differentierend regelaar*/
    output += params.kd * (error - lastErr);
    lastErr = error; 

    //  motoren aansturing & begrenzen
    output = constrain(output, -510, 510); 
    
    int powerLeft = 0;
    int powerRight = 0;
    
    if (run) if (output >= 0)
    {
      powerLeft = constrain(params.power + params.diff * output, -255, 255); 
      powerRight = constrain(powerLeft - output, -255, 255); 
      powerLeft = powerRight + output; 
    }
    else
    {
      powerRight = constrain(params.power - params.diff * output, -255, 255); 
      powerLeft = constrain(powerRight + output, -255, 255); 
      powerRight = powerLeft - output;
    }

   /*SerialPort.print("powerLeft");
    SerialPort.println(powerLeft);
    
    SerialPort.print("powerRight"); 
    SerialPort.println(powerRight);*/
  
    //H-brug waardes wegschrijven 
    
    analogWrite(AIN1, powerLeft > 0 ? powerLeft : 0);
    analogWrite(AIN2, powerLeft < 0 ? -powerLeft : 0);
    analogWrite(BIN1, powerRight > 0 ? powerRight : 0);
    analogWrite(BIN2, powerRight < 0 ? -powerRight : 0);
  } 

  unsigned long difference = micros() - current;
  if (difference > calculationTime) calculationTime = difference;
}

void onRun()
{
  run =true;
  digitalWrite(led, HIGH); 
}

void onStop()
{
  run =false; 
  digitalWrite(led, LOW);
}

void onCalibrate()
{
  char* param = sCmd.next();

  if (strcmp(param, "black") == 0)
  {
    SerialPort.print("start calibrating black... ");
    for (int i = 0; i < 6; i++) params.black[i] = analogRead(sensor[i]);   
    SerialPort.println("done");
    
  }
  else if (strcmp(param, "white") == 0)
  {
    SerialPort.print("start calibrating white... ");
    for (int i = 0; i < 6; i++) params.white[i] = analogRead(sensor[i]);   
    SerialPort.println("done");
  }
  EEPROM_writeAnything(0, params);
}

void onSet()
{
  char* param = sCmd.next();
  char* value = sCmd.next();


  // ki
  if (strcmp(param, "cycle") == 0)
  {
    long newCycleTime = atol(value);
    float ratio = ((float) newCycleTime) / ((float) params.cycleTime);

    params.ki *= ratio;
    params.kd /= ratio; 

    params.cycleTime = newCycleTime;
  }
  
  else if (strcmp(param, "ki") == 0)
  {
    float cycleTimeInSec = ((float) params.cycleTime) / 1000000;
    params.ki = atof(value) * cycleTimeInSec;
  }
  else if (strcmp(param, "kd") == 0)
  {
    float cycleTimeInSec = ((float) params.cycleTime) / 1000000;
    params.kd = atof(value) / cycleTimeInSec;
  }


  if (strcmp(param, "cycle") == 0) params.cycleTime = atol(value); // if 
  else if (strcmp(param, "power") == 0) params.power = atol(value); 
  else if (strcmp(param, "diff") == 0) params.diff = atof(value);
  else if (strcmp(param, "kp") == 0) params.kp = atof(value);
  //else if (strcmp(param, "ki") == 0) params.ki = atof(value);
  //else if (strcmp(param, "kd") == 0) params.kd = atof(value);
    
    /* parameters een nieuwe waarde geven via het set commando doe je hier ... */
  EEPROM_writeAnything(0, params);
}

void onDebug()
{
  SerialPort.print("cycle time: ");
  SerialPort.println(params.cycleTime);
  
  /* parameters weergeven met behulp van het debug commando doe je hier ... */
  SerialPort.print("black: ");
  for (int i = 0; i < 6; i++)
  {
    SerialPort.print(params.black[i]);
    SerialPort.print(" ");
  }
  SerialPort.println(" ");

  SerialPort.print("white: ");
  for (int i = 0; i < 6; i++)
  {
    SerialPort.print(params.white[i]);
    SerialPort.print(" ");
  }
  SerialPort.println(" ");

  SerialPort.print("normalised: ");
  for (int i = 0; i < 6; i++)
  {
    SerialPort.print(normalised[i]);
    SerialPort.print(" "); 
  }
  SerialPort.println(" ");

  SerialPort.print("output: ");
  SerialPort.println(output); 

  SerialPort.print("power: ");
  SerialPort.println(params.power);
  SerialPort.print("diff: ");
  SerialPort.println(params.diff);
  SerialPort.print("kp: ");
  SerialPort.println(params.kp);

  float cycleTimeInSec = ((float) params.cycleTime) / 1000000;
  float ki = params.ki / cycleTimeInSec;
  SerialPort.print("ki: ");
  SerialPort.println(ki);

  float kd = params.kd * cycleTimeInSec;
  SerialPort.print("kd: ");
  SerialPort.println(kd);
  
 SerialPort.print("Normalised Values Sensoren: ");
  for (int i = 0; i < 6; i++)
  {
    normalised[i] = map(analogRead(sensor[i]), params.black[i], params.white[i], 0, 1000);
    SerialPort.print(normalised[i]);
    SerialPort.print(" ");
  }
  SerialPort.println();

  SerialPort.print("Calculation Time: ");
  SerialPort.println(calculationTime);
  calculationTime = 0;
}

void onUnknownCommand(char *command)
{
  SerialPort.print("unknown command: \"");
  SerialPort.print(command);
  SerialPort.println("\""); 
}
