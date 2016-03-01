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
 *  7. upload to ESP8266 and enjoy!
 *  
 */

//MMB Libraries
#include <ESP8266MMB.h>
#include <ESP8266MMBJsonParser.h>

//ESP8266 Libray
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>


//---MMB SETTINGS---
//MMB account name
#define MMB_ACCOUNT_NAME ""
//name of your API
#define MMB_API_NAME ""

//---WIFI SETTINGS
//SSID
#define MY_SSID ""
//password
#define MY_PASS ""


//EXECUTION COUNT
int numRuns = 1;   // Execution count, so this doesn't run forever
int maxRuns = 1;   // Maximum number of times the Choreo should be executed

//MMB object
ESP8266MMB mmb;

//WiFiMulti object
ESP8266WiFiMulti WiFiMulti;

void setup()
{
	Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  WiFiMulti.addAP(MY_SSID, MY_PASS);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(MY_SSID);
  
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");

  Serial.println(F("\n--START--\n"));

  //set account name and api name
  mmb.setAccountName(MMB_ACCOUNT_NAME);
  mmb.setAPIName(MMB_API_NAME);

  //set API parameter
  mmb.addQueryStringParameter("city", "Ceggia");
  
  //run
  if (mmb.run()) { //se la chiamata ha avuto successo

    //instantiate parser
    ESP8266MMBJsonParser result;

    //parse the response
    result.parseJson(mmb.getResponse());

    if (result.success()) { //if the response is parsed correctly
      
      if (result.getStatusCode() == MMBJSON_OK) { //if API was successfully executed

        //print API data
        Serial.print(F("\nWEATHER: "));
        Serial.print((const char *) result.getData("main", "weather"));
        Serial.print(F("\n"));

        Serial.print(F("\nTEMPERATURE: "));
        Serial.print((const char *) result.getData("temp", "temp"));
        Serial.print(F("°C\n"));

        Serial.print(F("\nHUMIDITY: "));
        Serial.print((const char *) result.getData("humid", "other"));
        Serial.print(F("%\n"));
        
      } else {
        Serial.println(F("\nAPI HAS ENCOUNTERED SOME ERRORS \n"));
      }
 
      
    } else {
     Serial.println(F("\nPARSING ERROR\n"));
    }
      
  }
  

}



void loop() {
  //not used in this example
}
