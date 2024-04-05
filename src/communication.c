#include "communication.h"

#define CURL_ERROR(str, ...) fprintf(stderr, "[ERROR] " str, ##__VA_ARGS__)

size_t write_callback(const char* ptr, const size_t size, const size_t nmemb, const void* userData)
{
	const size_t realSize = size * nmemb;
	char** responsePtr = (char**)userData;

	const char* newPtr = realloc(*responsePtr, realSize + 1); // Reallocate memory for the response buffer
	if (newPtr) {
		*responsePtr = newPtr;
		memcpy(*responsePtr, ptr, realSize); // Copy the received data into the response buffer
		(*responsePtr)[realSize] = '\0';     // Null-terminate the response buffer
	}
	else {
		fprintf(stderr, "Memory reallocation failed\n");
		return EXIT_FAILURE;
	}

	return realSize;
}

char* getResponse(const char* ipAddress)
{
	CURL* curl = curl_easy_init();

	if (!curl) {
		CURL_ERROR("Failed to initialize cURL.\n");
		return NULL;
	}

	char* responseBuffer = NULL;
	const char url[256] = { NULL };

	snprintf(url, sizeof(url), "https://ipinfo.io/%s/geo", ipAddress);

	// Set cURL options
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);

	const CURLcode res = curl_easy_perform(curl); // Perform cURL request

	if (res != CURLE_OK) {
		CURL_ERROR("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

		// Clean up cURL resources and free memory
		curl_easy_cleanup(curl);
		free(responseBuffer);
		return NULL;
	}

	curl_easy_cleanup(curl);
	return responseBuffer;
}

void printJson(const char* response, const char* name)
{
	// Parse JSON response
	const cJSON* root = cJSON_Parse(response);

	if (!root) {
		printf("Error on parsing JSON data\n");
		return;
	}

	// Get the parameter from the JSON object
	const cJSON* param = cJSON_GetObjectItem(root, name);

	if (param)
		printf("%s: %s\n", name, param->valuestring);
	else
		printf("Field: '%s' not found.\n", name);

	// Delete the cJSON object to free memory
	cJSON_Delete(root);
}