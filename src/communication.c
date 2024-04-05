#include "communication.h"

#define CURL_ERROR(str, ...) fprintf(stderr, "[ERROR] " str, ##__VA_ARGS__)

size_t write_callback(const char* ptr, const size_t size, const size_t nmemb, const void* userdata)
{
	const size_t realSize = size * nmemb;
	char** responsePtr = (char**)userdata;

	const char* newPtr = realloc(*responsePtr, realSize + 1);
	if (newPtr)
	{
		*responsePtr = newPtr;
		memcpy(*responsePtr, ptr, realSize);
		(*responsePtr)[realSize] = '\0';
	}
	else
	{
		fprintf(stderr, "Memory reallocation failed\n");
		return EXIT_FAILURE;
	}

	return realSize;
}

char* getResponse(const char* ip_address)
{
	CURL* curl = curl_easy_init();

	if (!curl) {
		CURL_ERROR("Failed to initialize cURL.\n");
		return NULL;
	}

	char* responseBuffer = NULL;
	char url[256];

	snprintf(url, sizeof(url), "https://ipinfo.io/%s/geo", ip_address);

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);

	CURLcode res = curl_easy_perform(curl);

	if (res != CURLE_OK)
	{
		CURL_ERROR("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

		curl_easy_cleanup(curl);
		free(responseBuffer);
		return NULL;
	}

	curl_easy_cleanup(curl);
	return responseBuffer;
}

void printJson(const char* response, const char* name)
{
	const cJSON* root = cJSON_Parse(response);

	if (!root) {
		printf("Error on parsing JSON data\n");
		return;
	}

	const cJSON* param = cJSON_GetObjectItem(root, name);

	if (param && cJSON_IsString(param))
		printf("%s: %s\n", name, param->valuestring);
	else
		printf("Field: '%s' not found.\n", name);

	cJSON_Delete(root);
}