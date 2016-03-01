/*
	MMB Library porting for ESP8266

	MANCA GESTIONE ERRORI CHIAMATA
*/

#ifndef ESP8266MMB_H
#define ESP8266MMB_H

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h> //ESP8266 HttpClient


//HTTP CODE
#define MMB_HTTP_SUCCESS 0
#define MMB_HTTP_ERROR -1

//---INTERNAL CONFIGURATION
//debug
#define DEBUG 1

#define MMB_API_HOSTNAME "api.mymicrobridge.com" //hostname API endpoint
#define MMB_API_PORT 80 //port API endpoint

#define QUERY_STRING_INITIAL_SIZE 20 //buffer parametri query string
#define URI_TEMPLATE_INITIAL_SIZE 20 //buffer parametri uri template
#define X_WWW_FORM_URLENCODED_INITIAL_SIZE 1 //buffer parametri x-www-form-urlencoded

#define ACCOUNT_NAME_INITIAL_SIZE 20 //buffer account name
#define API_NAME_INITIAL_SIZE 20 //buffer API name

//MMB class
class ESP8266MMB {
	
	public:

		ESP8266MMB(); //costructor
		ESP8266MMB(int accountNameBufferSize, int apiNameBufferSize, int queryStringBufferSize, int uriTemplateBufferSize, int xWWWFormUrlencodedBufferSize); //costructor
		~ESP8266MMB(); //destroyer

		void setAccountName(const char *account); //set account name
		void setAPIName(const char *api); //set API name

		int run(); //execute API (make HTTP request)

		//lettura risposta
		//int available();
		//int read();
		//void close();
		String getResponse();

		//aggiunta parametri
		void addQueryStringParameter(const char *offset, const char *value); //query string
		void addUriTemplateParameter(const char *value); //uri template //DEVONO ESSERE INSERITI IN ORDINE
		void addXWWWFormUrlencodedParameter(const char *offset, const char *value); //x-www-form-urlencoded

		#ifdef DEBUG
			//---DEBUG PUBLIC---
			void printDataDebug();
		#endif


	private:

		void buildApiURL(char *url); //build API URL
		void buildUrlencodedParameter(char *queryString, const char *offset, const char *value); //costruisce il parametro queryString

		//---URLENCODE FUNCTION
		const char *_specialCharacters = "$&+,/:;=?@ <>#%{}|~[]`";

		static char hex_digit(char c) {
			return "01234567890ABCDEF"[c & 0x0F];
		}

		void urlencode(char *dst, const char *src); //esegue l'urlencode della stringa
		int countCharacterToUrlencode(const char *str); 

		//---EXECUTE
		int execute(char *url); //segue la chiamata

		//client HTTP da usare
		HTTPClient _http;

		char *_accountName; //user account name
		int _accountNameSize; //dimensione buffer

		char *_apiName; //user API name
		int _apiNameSize; //dimensione buffer

		//buffer QueryString
		char *_queryString;
		int _queryStringSize;

		//buffer UriTemplate
		char *_uriTemplate;
		int _uriTemplateSize;

		//buffer x-www-form-urlencoded parameter
		char *_xWWWFormUrlencoded;
		int _xWWWFormUrlencodedSize;


		#ifdef DEBUG
			//---DEBUG PRIVATE---
			void debugPrint(String msg);
		#endif


};

#endif