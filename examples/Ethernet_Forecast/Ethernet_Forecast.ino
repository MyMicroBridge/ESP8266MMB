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
 *  7. upload to Arduino/Fishino with Ethernet shield and enjoy!
 *  
 */

//MMB Libraries
#include <MMB.h>
#include <MMBJsonParser.h>

//Ethernet Libraies
#include <SPI.h>
#include <Ethernet.h>

//Flash library
#include <Flash.h>


//---MMB SETTINGS---
//MMB account name
#define MMB_ACCOUNT_NAME ""
//name of your API
#define MMB_API_NAME ""

//---ETHERNET SETTINGS
//MAC
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);

//EthernetClient
EthernetClient client;

//EXECUTION COUNT
int numRuns = 1;   // Execution count, so this doesn't run forever
int maxRuns = 1;   // Maximum number of times the Choreo should be executed

//MMB object
MMB mmb(client);

void setup()
{
  // Initialize serial and wait for port to open
  // Inizializza la porta seriale e ne attende l'apertura
  Serial.begin(9600);
  
  // only for Leonardo needed
  // necessario solo per la Leonardo
  while (!Serial)
    ;

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);

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

