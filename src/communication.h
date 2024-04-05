#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <curl/curl.h>
#include <cJSON/cJSON.h>

size_t write_callback(const char* ptr, const size_t size, const size_t nmemb, const void* userData);
char* getResponse(const char* ipAddress);
void printJson(const char* response, const char* name);

#endif // COMMUNICATION_H