/*
 * main.cpp
 *
 *  Created on: Jun 29, 2013
 *      Author: klpeter2
 */

#include "Server.h"
#include <boost/asio.hpp>

/*
 * IP address of the introducer process.
 *
 * This is the process that allows other
 * processes to join the group!
 *
 */
const std::string INTROCUCER_IP = "linux6.ews.illinois.edu";

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		std::cout << "Usage: This machines name <ip>" << std::endl;
		return 0;
	}

	// Create server
	boost::asio::io_service ioService;
	Server server(ioService, argv[1]);
	server.setIntroducerIp(INTROCUCER_IP);
	server.start();
	ioService.run();

	return 0;
}


