# Gebruiksaanwijzing

### opladen / vervangen batterijen
In dit project wordt gebruik gemaakt van batterijen met een extrene oplader. 
- Batterijen uit de linefollower halen
  Haal de batterijhouder uit het daarvoor voorziene slot van de linefollower. 
  [IMG20221212152149](https://user-images.githubusercontent.com/114672965/207071071-a07fcbb9-389a-4d1c-9a5f-7480e3e7aba7.jpg)
  Verwijder hierna de batterijen uit de batterijhouder.
- Batterijen opladen
  Plaats beide batterijen in de batterijoplader en verbind deze via de usb-kabel met de nodige dongle aan een stekker. 
  !Gebruik niet de usb-poort van een computer, deze kan de nodige stroomsterktes niet leveren.
  [IMG20221212152259](https://user-images.githubusercontent.com/114672965/207071800-d9c8e2e4-846f-4dad-bd0e-e0f15f4c5d6e.jpg)
  Verwijder na het opladen da batterijen van de oplader.
  !Lees aandachtig de handleiding van de oplader voor gebruik!
- Plaatsen van de batterijen
  Eens de batterijen verwijdert zijn uit de oplader kunnen deze teruggeplaatst worden in de batterijhouder. Plaats de batterijen terug in de batterijhouder, rekeninghoudend met de polariteit van de batterijen. De plus op de batterij moet overeenkomen met de plus op de batterijhouder. Plaats de batterijhouder terug in het daarvoor voorziene slot op de linefollower.
  
### draadloze communicatie
#### Smartphone verbinding maken met linefollower
Om verbinding te kunnen maken tussen een smartphone en linefollower dient men de app Serial Bleuthooth Terminal te installeren, deze is te vinden in de Play Store. Vooraleer er verbinding gameekt kan worden moet de linefollower van stroom voorzien zijn en de bleutooth op de smartphone aanstaan. Zolang er geen verbinding is zal er een rood en blauw licht knipperen. 
Open de app, druk op de drie lijnen linksbovenaan (groen) en kies voor device.
![Screenshot_2022-12-12-15-53-11-98_5b905283389ac51001251bb3d6ad9937](https://user-images.githubusercontent.com/114672965/207078511-8a25c032-a6ac-4369-b344-67f105dcdbc3.jpg)
Kies als device voor HC-05. De verbinding gebeurt nu automatisch. Indien de verbinding verloren gaat, druk dan in de terminal op het dubbele stekker icoon (Rood).
![Screenshot_2022-12-12-15-53-36-88_5b905283389ac51001251bb3d6ad9937](https://user-images.githubusercontent.com/114672965/207078735-31624576-d904-421c-9766-ca2768933e31.jpg)
De terminal zal aangeven dat deze geconecteerd is met de linefollower.
![Screenshot_2022-12-12-15-53-30-03_5b905283389ac51001251bb3d6ad9937](https://user-images.githubusercontent.com/114672965/207078711-2b88a08f-0ecf-4edf-bc05-1186c47dc713.jpg)
Onderaan kunnen commando's getypt worden en doorgestuurd worden naar de linefollower. De grijze boxen kunnen ingesteld worden als presets door deze ingedrukt te houden.
De verbinding kan verbroken worden door op het icoon met de twee stekkers te drukken(Rood).

#### commando's
debug [on/off] : Hiermee kunnen de ingestelde paramteres opgevraagd worden. 
run: De linefollower begint te rijden.
stop: De linefollower stopt met rijden.
set cycle [µs]: Om de hoeveel tijd het programma uitgevoerd wordt. deze dient steeds groter te zijn dan de calculation time.
set power [0..255]  Het maximaal vermogen dat de motoren kunnen gebruiken, indien deze te laag is zal de linefollower niet rijden.
set diff [0..1]  Deze beinvloed de snelheid in de bochten.
set kp [0..]: De mate waarin de linefollower een uitwijking zal corrigeren.
set ki [0..]: De mate waarin een fout die zich langdurig voorduit gecorrigeerd zal worden.
set kd [0..]: De mate waarin snelle veranderingen worden gecorrigeerd.  
calibrate black: De sensor slaat de waarden op die als zwart gezien worden.
calibrate white: De sensor slaat de waarden op die als wit gezien worden.

### kalibratie
Vooraleer de linefollower kan rijden dient de sensor welke lichtwaardes te volgen, daarom dient deze gekalibreert te worden. 
- Plaats de sensor volledig op het vlak waar de line zich in bevind en stuur het commando 'calibrate white'. Eens de response 'calibrate white ... Done' te voorschijn komt zijn deze waarden opgeslagen.
- Plaats de sensor volledig op een vlak met dezelfde kleur als de line en stuur het commando 'calibrate black'. Eens de response 'calibrate black ... Done' te voorschijn komt zijn deze waarden opgeslagen.

### settings
De robot rijdt stabiel met volgende parameters:  
cycle [µs]  4000
power [0..255]  50
diff [0..1]  0.5
kp [0..]: 6
ki [0..]: 0
kd [0..]: 0.25

### start/stop button
De manuele start/stop knop (rood) bevind zich op de printplaat naast de led. Deze knop doet de linefollower starten en stoppen. De Linefollower is ook voorzien van een schakelaar die de linefollower van stroom voorziet, deze bevind zich naast de batterijhouder (Groen).
![IMG20221212151950](https://user-images.githubusercontent.com/114672965/207085077-c368703a-0694-4973-ae31-7be1354f915d.jpg)


