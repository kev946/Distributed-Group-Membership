#include "Server.h"

/*
 * Main entry point to the server executable
 */
int main() {
	Server server;

	// Listen for messages on port "..."
	server.listen(Constants::get_port());

	return 0;
}
