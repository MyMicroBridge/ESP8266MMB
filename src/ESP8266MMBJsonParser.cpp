#include "Arduino.h"

#include "ESP8266MMBJsonParser.h"
	
//---PUBLIC---

//costruttore
ESP8266MMBJsonParser::ESP8266MMBJsonParser():ESP8266MMBJsonParser(JSON_MESSAGE_INITIAL_SIZE) {}

ESP8266MMBJsonParser::ESP8266MMBJsonParser(int dim) {

	_jsonMessage = (char *) malloc(dim * sizeof(char)); //alloco la memoria

	//inserisco il terminatore in _jsonMessage
	_jsonMessage[0] = 0;

	//azzero l'indice
	_index = 0;

	//indica se ci sonon
	_openQuotes = 0;

	//inizializzo il siccesso del parsing a 0
	_parseSuccess = MMBJSON_PARSE_ERROR; //ci sono errori
}

//destroyer
ESP8266MMBJsonParser::~ESP8266MMBJsonParser() {
	free(_jsonMessage);
}


//---PARSE FUNCTION
void ESP8266MMBJsonParser::parseJson(const char *message) { //devono essere già stati eliminati \n \t e spazi
	
	//inserisco tutti i caratteri nel buffer interno
	for (int i = 0; i < strlen(message); i++) {
		addCharacter(message[i]);
	}

	//inserisco il terminatore
	addCharacter('\0');

	//eseguo il parsing
	parseJson();
}

void ESP8266MMBJsonParser::parseJson(String message) { //elimino \n \t e spazi
	
	//inserisco tutti i caratteri nel buffer interno
	for (int i = 0; i < message.length(); i++) {
		addCharacter(message.charAt(i));
	}

	//inserisco il terminatore
	addCharacter('\0');

	//eseguo il parsing
	parseJson();
}

void ESP8266MMBJsonParser::parseJson() { //elimino \n \t e spazi

	#ifdef DEBUG
		debugPrint(F("\n\n---MESSAGE TO PARSE---\n\n"));
		debugPrint(_jsonMessage);
	#endif

	//creo json object
	_json = &_jsonBuffer.parseObject(_jsonMessage);

	if (_json->success()) {
		#ifdef DEBUG
			debugPrint(F("\nPARSE SUCCESS\n"));
		#endif
		_parseSuccess = MMBJSON_PARSE_SUCCESS;

	} else {
		#ifdef DEBUG
			debugPrint(F("\nPARSE ERROR\n"));
		#endif
		
		_parseSuccess = MMBJSON_PARSE_ERROR;
	}
}


//---GET FUNCTION
int ESP8266MMBJsonParser::getStatusCode() {
	return (*_json)["responses"][0]["status"]["code"];
}

JsonVariant ESP8266MMBJsonParser::getData(const char *key) {
	return getData(key, "default");
}

JsonVariant ESP8266MMBJsonParser::getData(const char *key, const char *nspace) {
	return (*_json)["responses"][0]["data"][nspace][key];
}

JsonVariant ESP8266MMBJsonParser::getErrors(int index, const char * nspace) {
	return (*_json)["responses"][0]["errors"][nspace][index];
}



//---ADD FUNCTION
void ESP8266MMBJsonParser::addCharacter(char c) {
	
	if (c == '\"') {
		_openQuotes = !_openQuotes;
	}

	if (_openQuotes) { //se ho già trovato \" devo tenere gli spazi

		if (c == '\n'  || c == '\t') { //elimino i caratteri \n e \t
			return;
		}

	} else { //altrimenti posso eliminarli

		if (c == '\n'  || c == '\t' || c == ' ') { //elimino i caratteri \n \t e spazio
			return;
		}
	}

	_jsonMessage[_index++] = c;
	_jsonMessage[_index] = 0;

}

//---SUCCESS
uint8_t ESP8266MMBJsonParser::success() {
	return _parseSuccess;
}

#ifdef DEBUG
	//---DEBUG---
	void ESP8266MMBJsonParser::debugPrint(String msg) {
		Serial.print(msg);
	}
#endif


