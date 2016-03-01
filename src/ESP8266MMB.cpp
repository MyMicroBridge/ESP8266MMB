#include "Arduino.h"

#include "ESP8266MMB.h"

//---PUBLIC---

ESP8266MMB::ESP8266MMB(): ESP8266MMB(ACCOUNT_NAME_INITIAL_SIZE, API_NAME_INITIAL_SIZE, QUERY_STRING_INITIAL_SIZE, URI_TEMPLATE_INITIAL_SIZE, X_WWW_FORM_URLENCODED_INITIAL_SIZE) {}

//costructor and create _http client
ESP8266MMB::ESP8266MMB(int accountNameBufferSize, int apiNameBufferSize, int queryStringBufferSize, int uriTemplateBufferSize, int xWWWFormUrlencodedBufferSize) {

	//inizializzo i buffer

	//apiName
	_apiName = (char *) malloc(apiNameBufferSize * sizeof(char)); //alloco la memoria
	_apiNameSize = apiNameBufferSize; //salvo la dimensione

	//accountName
	_accountName = (char *) malloc(accountNameBufferSize * sizeof(char)); //alloco la memoria
	_accountNameSize = accountNameBufferSize; //salvo la dimensione

	//queryString
	_queryString = (char *) malloc(queryStringBufferSize * sizeof(char)); //alloco la memoria
	_queryStringSize = queryStringBufferSize; //salvo la dimensione
	_queryString[0] = 0; //inserisco il carattere vuoto all'inizio

	//uriTemplate
	_uriTemplate = (char *) malloc(uriTemplateBufferSize * sizeof(char)); //alloco la memoria
	_uriTemplateSize = uriTemplateBufferSize; //salvo la dimensione
	_uriTemplate[0] = 0; //inserisco il carattere vuoto all'inizio

	//uriTemplate
	_xWWWFormUrlencoded = (char *) malloc(xWWWFormUrlencodedBufferSize * sizeof(char)); //alloco la memoria
	_xWWWFormUrlencodedSize = xWWWFormUrlencodedBufferSize; //salvo la dimensione
	_xWWWFormUrlencoded[0] = 0; //inserisco il carattere vuoto all'inizio
	
}

//destroyer
ESP8266MMB::~ESP8266MMB() {

	//libero la memoria allocata dinamicamente
	free(_accountName);
	free(_apiName);

	free(_queryString);
	free(_uriTemplate);
	free(_xWWWFormUrlencoded);

}

//---SET FUNZIONI INIZIALI
void ESP8266MMB::setAccountName(const char *account) { //account name

	#ifdef DEBUG
		debugPrint(F("\n SIZE OF _accountName: "));
		debugPrint(String(_accountNameSize));

		debugPrint(F("\n SIZE OF account: "));
		debugPrint(String(strlen(account) +1));
		debugPrint(F("\n"));
	#endif
	
	//se la dimensione del buffer è troppo piccola la realloco
	if (_accountNameSize < (strlen(account) +1)) {
		_accountName = (char *) realloc(_accountName, (strlen(account) +1) * sizeof(char));
		_accountNameSize = strlen(account) +1;
	}

	strcpy(_accountName, account);

}

void ESP8266MMB::setAPIName(const char *api) { //api name

	#ifdef DEBUG
		debugPrint(F("\n SIZE OF _apiName: "));
		debugPrint(String(_apiNameSize));

		debugPrint(F("\n SIZE OF api: "));
		debugPrint(String(strlen(api) +1));
		debugPrint(F("\n"));
	#endif
	
	//se la dimensione del buffer è troppo piccola la realloco
	if (_apiNameSize < (strlen(api) +1)) {
		_apiName = (char *) realloc(_apiName, (strlen(api) +1) * sizeof(char));
		_apiNameSize = strlen(api) +1;
	}

	strcpy(_apiName, api);
}

//---AGGIUNTA PARAMETRI API
void ESP8266MMB::addQueryStringParameter(const char *offset, const char *value) { //query string

	if (_queryString[0] == 0) { //se query string è vuota

		//size della stringa per contenere i dati
		int size = strlen(offset) + strlen(value) + countCharacterToUrlencode(value)*2 + 2; //compresi = e terminatore di stringa

		if (_queryStringSize < size) {
			//realloco il buffer e salvo la nuova dimensione
			_queryString = (char *) realloc(_queryString, size * sizeof(char));
			_queryStringSize = size;
		}

	} else {

		int size = strlen(offset) + strlen(value) + countCharacterToUrlencode(value)*2 + 3; //compresi = e & e terminatore di stringa

		if ((_queryStringSize - strlen(_queryString) +1) < size) {

			//salvo il contenuto del buffer
			char tempBuffer[strlen(_queryString) +1];
			strcpy(tempBuffer, _queryString);

			//realloco il buffer e salvo la nuova dimensione
			_queryString = (char *) realloc(_queryString, (strlen(_queryString) + size) * sizeof(char));
			_queryStringSize = strlen(_queryString) + size;

			//ricopio il contenuto del buffer
			strcpy(_queryString, tempBuffer);
		}

		strcat(_queryString, "&\0"); //aggiungo il carattere di separazione
	}

	buildUrlencodedParameter(_queryString, offset, value); //aggiungo il parametro

}

void ESP8266MMB::addUriTemplateParameter(const char *value) { //uri template //DEVONO ESSERE INSERITI IN ORDINE

	if (_uriTemplate[0] == 0) { //se query string è vuota

		//size della stringa per contenere i dati
		int size = strlen(value) + countCharacterToUrlencode(value)*2 + 1; //compreso terminatore di stringa

		if (_uriTemplateSize < size) {
			//realloco il buffer e salvo la nuova dimensione
			_uriTemplate = (char *) realloc(_uriTemplate, size * sizeof(char));
			_uriTemplateSize = size;
		}

	} else {

		int size = strlen(value) + countCharacterToUrlencode(value)*2 + 2; //compresi = e & e terminatore di stringa

		if ((_uriTemplateSize - strlen(_uriTemplate) +1) < size) {

			//salvo il contenuto del buffer
			char tempBuffer[strlen(_uriTemplate) +1];
			strcpy(tempBuffer, _uriTemplate);

			//realloco il buffer e salvo la nuova dimensione
			_uriTemplate = (char *) realloc(_uriTemplate, (strlen(_uriTemplate) + size) * sizeof(char));
			_uriTemplateSize = strlen(_uriTemplate) + size;

			//ricopio il contenuto del buffer
			strcpy(_uriTemplate, tempBuffer);
		}

		strcat(_uriTemplate, "/\0"); //aggiungo il carattere di separazione
	}

	//posizione da cui inserire value urlencoded
	int position = strlen(_uriTemplate);

	//inserisco la stringa dopo la conversione
	urlencode(&_uriTemplate[position], value);
	
}

void ESP8266MMB::addXWWWFormUrlencodedParameter(const char *offset, const char *value) { //x-www-form-urlencoded

	if (_xWWWFormUrlencoded[0] == 0) { //se query string è vuota

		//size della stringa per contenere i dati
		int size = strlen(offset) + strlen(value) + countCharacterToUrlencode(value)*2 + 2; //compresi = e terminatore di stringa

		if (_xWWWFormUrlencodedSize < size) {
			//realloco il buffer e salvo la nuova dimensione
			_xWWWFormUrlencoded = (char *) realloc(_xWWWFormUrlencoded, size * sizeof(char));
			_xWWWFormUrlencodedSize = size;
		}

	} else {

		int size = strlen(offset) + strlen(value) + countCharacterToUrlencode(value)*2 + 3; //compresi = e & e terminatore di stringa

		if ((_xWWWFormUrlencodedSize - strlen(_xWWWFormUrlencoded) +1) < size) {

			//salvo il contenuto del buffer
			char tempBuffer[strlen(_xWWWFormUrlencoded) +1];
			strcpy(tempBuffer, _xWWWFormUrlencoded);

			//realloco il buffer e salvo la nuova dimensione
			_xWWWFormUrlencoded = (char *) realloc(_xWWWFormUrlencoded, (strlen(_xWWWFormUrlencoded) + size) * sizeof(char));
			_xWWWFormUrlencodedSize = strlen(_xWWWFormUrlencoded) + size;

			//ricopio il contenuto del buffer
			strcpy(_xWWWFormUrlencoded, tempBuffer);
		}

		strcat(_xWWWFormUrlencoded, "&\0"); //aggiungo il carattere di separazione
	}

	buildUrlencodedParameter(_xWWWFormUrlencoded, offset, value); //aggiungo il parametro
}

//---LETTURA DELLA RISPOSTA
// int ESP8266MMB::available() {
// 	return _http.available();
// }

// int ESP8266MMB::read() {
// 	return _http.read();
// }

// void ESP8266MMB::close() {
// 	return _http.stop();
// }
String ESP8266MMB::getResponse() {
	return _http.getString();
}

//---ESECUZIONE API (CHIAMATA HTTP)
int ESP8266MMB::run() {
	#ifdef DEBUG
		debugPrint(F("Running..............."));
	#endif

	//size della stringa contenente l'url
	int size = 1; //spazio per il carattere terminatore

	//dimensione accountName
	size += strlen(_accountName) +1; //compreso / iniziale

	//dimensione apiName
	size += strlen(_apiName) +1; //compreso / iniziale

	//dimensione uriTemplate
	if (_uriTemplate[0] != 0) { //se sono presenti parametri uriTemplate
		size += strlen(_uriTemplate) +1; //compreso / iniziale

	}

	//dimensione queryString
	if (_queryString[0] != 0) { //se sono presenti parametri queryString
		size += strlen(_queryString) +2; //compresi /? iniziali
	}

	#ifdef DEBUG
		debugPrint(F("\nSIZE of URL: "));
		debugPrint(String(size));
		debugPrint(F("\n"));
	#endif

	//inzializzo la variabile contenente l'url
	char url[size];

	url[0] = 0; //inserisco il terminatore di stringa

	buildApiURL(url); //costruisco l'url
	return execute(url); //eseguo la chiamata http
}



//---PRIVATE---


//---EXECUTE
int ESP8266MMB::execute(char *url) {

	int httpCode = 0;

	#ifdef DEBUG
		debugPrint(F("\nURL: "));
		debugPrint(url);
		debugPrint(F("\nURL SIZE: "));
		debugPrint(String(strlen(url) +1));
		debugPrint(F("\n"));
	#endif

	//---START REQUEST
	_http.begin(MMB_API_HOSTNAME, MMB_API_PORT, url); //HTTP

	if (_xWWWFormUrlencoded[0] != 0) { //chiamata POST (con body)

		//ricopio il body in un array di uint8_t
		uint8_t payload_uint8_t[strlen(_xWWWFormUrlencoded)];
		memcpy(payload_uint8_t, _xWWWFormUrlencoded, strlen(_xWWWFormUrlencoded));
        
        //---CHIAMATA POST
		httpCode = _http.POST(payload_uint8_t, strlen(_xWWWFormUrlencoded));

	} else { //non esistono parametri da inviare nel body della richiesta (GET)

		//---CHIAMATA GET
		httpCode = _http.GET();
	}



	if (httpCode == 200) { //se la chiamata è andata a buon fine

		#ifdef DEBUG
			debugPrint(F("\nSUCCESS!\n"));
			debugPrint(F("STATUS CODE: "));
			debugPrint(String(httpCode));
			debugPrint(F("\n"));
			debugPrint(F("\n"));
		#endif

		return MMB_HTTP_SUCCESS;

	} else {

		#ifdef DEBUG
			debugPrint(F("\nERROR!\n"));
			debugPrint(F("ERROR: "));
			debugPrint(String(httpCode));
			debugPrint(F("\n"));
			debugPrint(F("\n"));
		#endif

		return MMB_HTTP_ERROR;
	}

}

//costruisce l'url della risorsa (API) con i parametri
void ESP8266MMB::buildApiURL(char *url) {

	//---BASE URL
	strcat(url, "/");
	strcat(url, _accountName);
	strcat(url, "/");
	strcat(url, _apiName);

	//---AGGIUNTA URI TEMPLATE
	if (_uriTemplate[0] != 0) {
		strcat(url, "/");
		strcat(url, _uriTemplate);
	}

	//---AGGIUNTA QUERY STRING
	if (_queryString[0] != 0) {
		strcat(url, "/?");
		strcat(url, _queryString);
	}

}

//costruisce e restituisce un parametro query string
void ESP8266MMB::buildUrlencodedParameter(char *str, const char *offset, const char *value) {

	//eseguo l'ulrencode dei parametri e li inserisco
	//inserisco il parametro 
	strcat(str, offset);
	strcat(str, "=\0");

	//posizione da cui inserire value urlencoded
	int position = strlen(str);

	//inserisco la stringa dopo la conversione
	urlencode(&str[position], value);
}

//---URLENCODE
void ESP8266MMB::urlencode(char *dst, const char *src) { //esegue l'urlencode della stringa
	char c,*d = dst;
	while (c = *src++) {  
		if (strchr(_specialCharacters, c)) {  
			*d++ = '%';
			*d++ = hex_digit(c >> 4);
			c = hex_digit(c);
		}
		*d++ = c;
	}
	*d = '\0';
}

int ESP8266MMB::countCharacterToUrlencode(const char *str) { //conta il numero di caratteri da convertire
	char c;
	int n = 0;

	while (c = *str++) {  
		if (strchr(_specialCharacters, c)) {  
        	n++;
		}
	}

	return n;
}


#ifdef DEBUG
	//---DEBUG---
	void ESP8266MMB::printDataDebug() {
		Serial.println();
		Serial.println(F("---------------DEBUG---------------"));

		Serial.print(F("BUFFER ACCOUNT NAME: "));
		Serial.println(_accountName);

		Serial.print(F("BUFFER API NAME: "));
		Serial.println(_apiName);

		Serial.print(F("BUFFER QUERY STRING PARAMS: "));
		Serial.println(_queryString);

		Serial.print(F("BUFFER URI TEMPLATE PARAMS: "));
		Serial.println(_uriTemplate);

		Serial.print(F("BUFFER X-WWW-FORM-URLENCODED PARAMS: "));
		Serial.println(_xWWWFormUrlencoded);

		Serial.println(F("---------------DEBUG---------------"));
		Serial.println();
	}

	void ESP8266MMB::debugPrint(String msg) {
		Serial.print(msg);

	}
#endif