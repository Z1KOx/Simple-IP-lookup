#include "communication.h"

int main()
{
	const char ip_address[25];

	printf("Enter an IP address to lookup for\n> ");
	scanf_s("%s", ip_address, (unsigned int)_countof(ip_address));
	printf("\n");

	const char* buffer = getResponse(ip_address);

	if (buffer)
	{
		const char* types[9] = {
			"ip", "hostname", "city", "region",
			"country", "loc", "org", "postal",
			"timezone"
		};

		for (int i = 0; i < 9; ++i)
			printJson(buffer, types[i]);

		free(buffer);
	}

	return EXIT_SUCCESS;
}
