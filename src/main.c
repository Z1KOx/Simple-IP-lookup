#include "communication.h"

int main()
{
	const char ipAddress[25];

	// Prompt user for IP address input
	printf("Enter an IP address to lookup for\n> ");
	scanf_s("%s", ipAddress, (unsigned int)_countof(ipAddress));
	printf("\n");

	// Retrieve response based on user input IP address
	const char* buffer = getResponse(ipAddress);

	if (buffer) 
	{
		// Define array of parameter types to extract from JSON response
		const char* types[9] = {
			"ip", "hostname", "city", "region",
			"country", "loc", "org", "postal",
			"timezone"
		};

		// Loop through parameter types and print corresponding values from JSON response
		for (int i = 0; i < 9; ++i)
			printJson(buffer, types[i]);

		// Free memory allocated for response buffer
		free(buffer);
	}

	return EXIT_SUCCESS;
}