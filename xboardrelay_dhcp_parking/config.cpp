#include "config.h"

// esto es lo que hay que cambiar por cada cosa

byte MAC[6] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0D};
const char* HOSTNAME = "relaypuerta"; 
const char* PASSWORD = "1234";
byte IP[4] = {192, 168, 0, 3};
const int PORT = 80;
const char* FOLDER = "/api/";
const String REQUEST_HOST = "74.125.232.128";   // numeric IP for Google (no DNS)
//char REQUEST_HOST[] = "www.google.com";   // name address for Google (using DNS)
const String REQUEST_QUERY = "/default_request/";
