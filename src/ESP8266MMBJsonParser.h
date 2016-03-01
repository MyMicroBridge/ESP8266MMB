/*
	MMBJsonParser Library

	MANCA GESTIONE ERRORI

	- ERRORE NEL PARSING DI STRING E CHAR ARRAY

*/

#ifndef MMBJSONPARSER_H
#define MMBJSONPARSER_H

#include "Arduino.h"
#include <ArduinoJson.h>

//PARSING CODE
#define MMBJSON_PARSE_SUCCESS 1
#define MMBJSON_PARSE_ERROR 0


//---STATUS CODE
//bad request 1xx
#define MMBJSON_USERNAME_NOT_FOUND 100
#define MMBJSON_API_NOT_FOUND 101
#define MMBJSON_PARAMETER_NOT_FOUND 102

//ok 2xx
#define MMBJSON_OK 200

//permission error 3xx
#define MMBJSON_HTTPS_CALL_NOT_ALLOWED 300
#define MMBJSON_DAILY_API_CALLS_LIMIT_EXCEEDED 301
#define MMBJSON_HOURLY_API_CALLS_LIMIT_EXCEEDED 302
#define MMBJSON_DATA_LIMIT_EXCEEDED 303
#define MMBJSON_API_CALLS_MUST_BE_PERFORMED_UNDER_HTTPS 304

//service error 4xx
#define MMBJSON_INTERNAL_SERVICE_ERROR 400

//server error 5xx
#define MMBJSON_INTERNAL_SERVER_ERROR 500

//response error 6xx
#define MMBJSON_INTERNAL_SOURCE_ERROR 600

//response error 7xx
#define MMBJSON_INTERNAL_RESPONSE_FORMAT_ERROR 700


//---INTERNAL CONFIGURATION
//debug
//#define DEBUG 1
	
#define JSON_MESSAGE_INITIAL_SIZE 130


//definisco gli errori


class ESP8266MMBJsonParser {

	public:

		ESP8266MMBJsonParser();
		ESP8266MMBJsonParser(int dim);
		~ESP8266MMBJsonParser(); //destroyer


		//---PASE FUNCTION
		void parseJson(const char *message);
		void parseJson(String message);
		void parseJson();

		//---GET FUNCTION
		int getStatusCode();

		JsonVariant getData(const char *key);
		JsonVariant getData(const char *key, const char *nspace);

		JsonVariant getErrors(int index, const char *nspace);

		//---ADD CHARACTER
		void addCharacter(char c);

		//---CHECK IF PARSING IS SUCCESSED
		uint8_t success();



	private:

		//buffer messaggio
		char *_jsonMessage;
		int _index = 0;
		uint8_t _openQuotes; //indica se è stato trovato il carattere"

		//buffer Json
		StaticJsonBuffer<500> _jsonBuffer;

		//json object
		JsonObject *_json;

		uint8_t _parseSuccess;

		#ifdef DEBUG
			//---DEBUG PRIVATE---
			void debugPrint(String msg);
		#endif


};




#endif