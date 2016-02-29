/*
 * MMB example
 * 
 * To use this example you must:
 *  1. register an account at members.mymicrobridge.com
 *  2. create an API with the service "OpenWeatherMap Forecast"
 *  3. fill the parameters list with your data ("city" parameter source must be "Query String (GET)"
 *  4. enable your new API
 *  5. fill MMB_ACCOUNT_NAME with your username at members.mymicrobridge.com
 *  6. fill MMB_API_NAME with your API "call name"
 *  7. upload to FISHINO UNO and enjoy!
 *  
 */


#include <MMB.h>
#include <MMBJsonParser.h>

#include <Flash.h>
#include <FishinoUdp.h>
#include <FishinoSockBuf.h>
#include <Fishino.h>
#include <SPI.h>


//---MMB SETTINGS---
//MMB account name
#define MMB_ACCOUNT_NAME ""
//name of your API
#define MMB_API_NAME ""

//---FISHINO SETTINGS
//SSID
#define MY_SSID	""
//password
#define MY_PASS	""


//FishinoClient
FishinoClient client;

//EXECUTION COUNT
int numRuns = 1;   // Execution count, so this doesn't run forever
int maxRuns = 1;   // Maximum number of times the Choreo should be executed

//MMB object
MMB mmb(client);

//WiFi Status
void printWifiStatus()
{
	// print the SSID of the network you're attached to:
	// stampa lo SSID della rete:
	Serial.print("SSID: ");
	Serial.println(Fishino.SSID());

	// print your WiFi shield's IP address:
	// stampa l'indirizzo IP della rete:
	IPAddress ip = Fishino.localIP();
	Serial << F("IP Address: ");
	Serial.println(ip);

	// print the received signal strength:
	// stampa la potenza del segnale di rete:
	long rssi = Fishino.RSSI();
	Serial << F("signal strength (RSSI):");
	Serial.print(rssi);
	Serial << F(" dBm\n");
}


void setup()
{
	// Initialize serial and wait for port to open
	// Inizializza la porta seriale e ne attende l'apertura
	Serial.begin(115200);
	
	// only for Leonardo needed
	// necessario solo per la Leonardo
	while (!Serial)
		;

  // initialize SPI
  // inizializza il modulo SPI
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  
  // reset and test WiFi module
  // resetta e testa il modulo WiFi
  while(!Fishino.reset())
    Serial << F("Fishino RESET FAILED, RETRYING...\n");
  Serial << F("Fishino WiFi RESET OK\n");

  // go into station mode
  // imposta la modalità stazione
  Fishino.setMode(STATION_MODE);

  // try forever to connect to AP
  // tenta la connessione finchè non riesce
  Serial << F("Connecting to AP...");
  while(!Fishino.begin(MY_SSID, MY_PASS))
  {
    Serial << ".";
    delay(2000);
  }
  Serial << F("OK\n");
  
  //Fishino in DHCP per ottenere il DNS
  Fishino.staStartDHCP();

  // wait till connection is established
  Serial << F("Waiting for IP...");
  while(Fishino.status() != STATION_GOT_IP)
  {
    Serial << ".";
    delay(500);
  }
  Serial << F("OK\n");

  //WiFi Status
  printWifiStatus();

  Serial << F("\n--START--\n");

  //set account name and api name
  mmb.setAccountName(MMB_ACCOUNT_NAME);
  mmb.setAPIName(MMB_API_NAME);

  //set API parameter
  mmb.addQueryStringParameter("city", "Ceggia"); //must be urlencoded if there are special characters (e.g. "San Donà di Piave" must be "San%20Donà%20di%20Piave")
  
  //run
  mmb.run();

}



void loop() {
  
  if (numRuns <= maxRuns) {

    //creo oggetto parser
    MMBJsonParser result;
    
    while (mmb.available())
    {
      //fill result with the data received by mmb.read()
      result.addCharacter(mmb.read());
      
    }

    //parse response
    result.parseJson();

    if (result.success()) { //if the response is parsed correctly
      
      if (result.getStatusCode() == MMBJSON_OK) { //if API was successfully executed

        //print API data
        Serial << F("\nWEATHER: ") << (const char *) result.getData("main", "weather") << F("\n");

        Serial << F("\nTEMPERATURE: ") << (const char *) result.getData("temp", "temp") << F("°C\n");
        
        Serial << F("\nHUMIDITY: ") << (const char *) result.getData("humid", "other") << F("%\n");
        
      } else {
        Serial.println(F("\nAPI HAS ENCOUNTERED SOME ERRORS \n"));
      }
 
      
    } else {
      Serial.println(F("\nPARSING ERROR\n"));
    }

    numRuns++;
    
  }
  
}
