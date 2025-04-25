# ESP32-Scanner-POE Ohjeet

## Käyttö
### Aloitus
ESP32 tarvitsee toimiakseen virtaa (USB tai POE) ja Serial feedin näkemiseen tarvitsee USB:n yhdistettynä tietokoneeseen. USB pitää olla yhdistettynä ESP:hen pinneihin IO44 (RX) ja IO43 (TX) sekä maahan, jotta Serial feediä tulee tietokoneeseen. GPS moduulin tulee olla yhdistettynä pinneihin IO17 (TXD1) ja IO18 (RXD1), jotta moduuli pystyy lähettämään Serial feediä ESP:lle.

### Serial
Serial feediä voi seurata millä tahansa sovelluksella, jolla Serial feediä voi seurata, esimerkiksi PuTTY. Serial monitorin asetuksista tulee varmistaa ensiksi oikea portti. Tämä löytyy Device Manager näkymästä Ports (COM & LPT) kohdasta. Portti on muodota COM[numero]. Baud rate tälle portille kuuluisi olla 9600. Jotta Serialissä näkyisi myös tärkeät alkuvaiheen logit, yhdistä USB johto tietokoneeseen, aloita Serial feedin monitorointi, irroita ESP moduulista virta ja laita heti takaisin. Näin Serialiin tulee näkyviin kaikki alkupuolen prosessit, kuten esimerkiksi ESP:lle annettu IP-osoite.

### OTA
Toivottavasti OTA-pävitykset toimivat. OTA-päivitykset tarvitsevat internet yhteyden toimiakseen. Yhdistä Serial USB tietokoneeseen ja anna moduulille virtaa Ethernet portin kautta. Seuraa Serial monitoria ja yritä löytää logien seasta Ethernet Network Event, joka näyttää tältä:
```
[ 10903][V][ETH.cpp:100] _onEthEvent(): eth0 Connected
[ 10961][V][NetworkEvents.cpp:117] _checkForEvent(): Network Event: 3 - ETH_CONNECTED
[ 15420][V][NetworkInterface.cpp:78] _onIpEvent(): eth0 Got New IP: 192.168.100.116 MASK: 255.255.255.0 GW: 192.168.100.1
[ 15549][V][NetworkEvents.cpp:117] _checkForEvent(): Network Event: 5 - ETH_GOT_IP
```
Jos tälläiset rivit löytyvät Serial feedistä, ennen kuin GPS tietoja alkaa tulla, ESP32 on yhdistänyt internettiin Ethernetin kautta. `eth0 Got New IP: 192.168.100.116` on ESP32:sen IP-osoite ja OTA-päivitys sivu löytyy osoitteesta `<ip-osoite>/update` esimerkiksi `192.168.100.116/update` Mikäli ESP32 joutuu OTA:n kautta päivittämään, raahaa `firmware.bin` tiedosto dropzonelle tai valitse tiedosto manuaalisesti. Päivitys tapahtuu automaattisesti

## Jos ei toimi...
### Ennen kuin kajotaan koodiin...
1. Varmista että GPS moduuli ja Serial USB johto on asennettu oikeihin pinneihin
    USB:
    - GPIO43 (TX)
    - GPIO44 (RX)
    
    GPS:
    - GPIO17 (TXD1)
    - GPIO18 (RXD1)
2. Varmista että seuraat varmasit oikeaa Serial porttia (COM[numero]). Tarkista COM-portin numero Device Manager näkymästä.
3. Varmista että ESP32 saa varmasti virtaa Ethernet portin kautta. Valoja pitäisi vilkkua

### PlatformIO/pioarduino
Jotta ESP32:sen lähdekoodia voi muuttaa, tarvitsee ensiksi olla asennettuna:
- VSCode
    - Lataa ja asenna [Visual Studio Code](https://code.visualstudio.com/), jos sitä ei ole vielä asennettu
    - Käynnistä sovellus
- pioarduino IDE
    - Avaa VSCoden Extensions sivuvalikko ikkunan vasemmasta reunasta ja hae laajennusta nimelta "pioarduino IDE"
    - Asenna pioarduino IDE laajennus
- git
    - Git tarvitsee olla asennettuna, jotta lähdekoodin voi ladata omalle koneelle. Seuraa asennusohjeita osoitteessa https://github.com/git-guides/install-git

### Lähdekoodin lataaminen
Navigoi haluamaasi kansioon, johon haluat ladata lähdekoodin ja avaa komentorivi kyseisessä kansiossa. Tämä onnistuu helposti navigoimalla haluamaan kansioon File Explorerissa, kirjoittamalla tiedostopolkukenttään ikkunan yläosassa "cmd" ja painamalla Enter. Näin cmd aukeaa kyseisessä kansiossa.

Kun Git on asennettu, aja seuraava komento:
```
git clone https://github.com/Missing-Link-harkat/ESP32-Scanner-POE
```
Tämä lataa kaikki tarvittavat tiedostot Githubista tietokoneelle.

### Projektin avaaminen
Avaa VSCode ja paina prosessorin näköistä kuvaketta ikkunan vasemmassa reunassa avataksesi pioarduino sivuvalikon. Ylimpänä pitäisi olla "Pick a folder"-nappula, josta voi valita jo olemassa olevan pioarduino projektikansion, joka sisältää `platformio.ini` tiedoston. Paina sitä, ja valitse juuri lataamasi lähdekoodin juurikansio.

Pioarduino mahdollisesti alkaa knofiguroimaan ja/tai latamaan esp32-arduino core kirjastoja perustuen `platformio.ini` tiedostosta löytyvään konfiguraatioon. Kun kaikki on ladattu/konfiguroitu, avaamalla pioarduino sivuvalikon, voi ohjelman kasata (build), ladata (upload) tai monitoroida (monitor).

Jos Serial monitoria haluaa seurata VSCodesta, muuta `platformio.ini` tiedostosta riviä:
```
monitor_port = COM[14]
```
valitsemaasi porttiin. Nyt painamalla "monitor" nappulaa pioarduinon sivuvalikosta, VSCode rupeaa kuuntelemaan kyseisen portin Serial feediä.

Projektin päätiedosto on `main.cpp`, joka löytyy `src` kansiosta.

Jos haluaa ladata uuden firmwaren OTA:n kautta ja valitse "Build" pioarduinon sivuvalikosta. Kun sovelluksen kokoaminen on valmis, etsi `firmware.bin` tiedosto rakennettujen tiedostojen joukosta, siirry OTA-päivityssivulle ja lataa firmware ESP:lle. `firmware.bin` tiedoston pitäisi yleensä löytyä kansiosta `<projektin juuri>/.pio/build/esp32-s3-devkitm-1/`

### Ethernetin korvaaminen WiFillä
Jos Ethernet ei toimi, eikä sitä saa toimimaan, mutta haluaa kuitenkin ESP:n yhdistävän internettiin, jotta OTA-päivitykset toimivat, korvaa kaikki `main.cpp` tiedoston koodi seuraavalla:
```
#include "Arduino.h"
#include "ElegantOTAPro.h"
#include "middleware.h"
#include "WiFi.h"
// #include "SPI.h"
// #include "ETH.h"

// #define USE_TWO_ETH_PORTS 0

// #define ETH_PHY_TYPE ETH_PHY_W5500
// #define ETH_PHY_ADDR 1
// #define CS_PIN       14
// #define RST_PIN      9
// #define INT_PIN      10

// #define MOSI_PIN     11
// #define MISO_PIN     12
// #define CLK_PIN      13

char ssid[] = "your-SSID"; // korvaa omalla SSID:lläsi
char password[] = "your-PASSWORD"; // korvaa SSID:si salasanalla

#define TXD1_PIN     17
#define RXD1_PIN     18

#define GPS_BAUD     9600

// byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

AsyncWebServer server(80);

HardwareSerial gpsSerial(1);

unsigned long lastTime = 0;
bool isConnected = false;

void setup() {
  Serial.begin(9600);
  delay(5000);
  // gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD1_PIN, TXD1_PIN);
  // Serial.println("Starting...");

  // SPI.begin(CLK_PIN, MISO_PIN, MOSI_PIN);
  // Serial.println("Initializing Ethernet...");
  // ETH.begin(ETH_PHY_TYPE, ETH_PHY_ADDR, CS_PIN, INT_PIN, RST_PIN, SPI);
  // Serial.println("Ethernet initialized");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  unsigned long timeout = millis();

  while (WiFi.status() != WL_CONNECTED && (millis() - timeout < 15000)) {
    delay(1000);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    isConnected = true;

    ElegantOTA.setID(Middleware.getDeviceID());
    ElegantOTA.setFWVersion(Middleware.getDeviceVersion());
    ElegantOTA.setDeveloperMode(Middleware.isDeveloperMode());
  
    ElegantOTA.begin(&server);
  
    server.begin();
    Serial.println("Starting server...");
    delay(10000);
  } else {
    Serial.println("Failed to connect to WiFi");
    return;
  }
}

void loop() {
  if (millis() - lastTime > 5000) {
    while (gpsSerial.available() > 0) {
      char gpsData = gpsSerial.read();
      Serial.print(gpsData);
    }
    lastTime = millis();
  }
  if (isConnected) {
    ElegantOTA.loop();
  }
}
```

Toivottavasti tällä pärjätään!