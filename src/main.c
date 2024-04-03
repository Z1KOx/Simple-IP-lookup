#include <stdio.h>
#include <curl/curl.h>

#define CURL_ERROR(str, ...) fprintf(stderr, "[ERROR] " str, ##__VA_ARGS__)

int main()
{
	char ip_address[20];

	printf("Enter an IP address to lookup for: ");
	scanf_s("%s", ip_address, (unsigned)_countof(ip_address));

	CURL* curl = curl_easy_init();

	if (!curl) {
		CURL_ERROR("Failed to initialize cURL.\n");
		return 1;
	}

	char url[256];
	snprintf(url, sizeof(url), "https://ipinfo.io/%s/geo", ip_address);

	curl_easy_setopt(curl, CURLOPT_URL, url);
	CURLcode res = curl_easy_perform(curl);

	if (res != CURLE_OK) {
		CURL_ERROR("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		curl_easy_cleanup(curl);
		return EXIT_FAILURE;
	}

	curl_easy_cleanup(curl);
	return 0;
}
