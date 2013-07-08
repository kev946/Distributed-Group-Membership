#include "Server.h"
using boost::asio::ip::tcp;

Server::Server() {

}

Server::~Server() {

}

/*
 * This method listens for a message sent from a client that
 * instructs it to create a logfile and return the results.
 *
 * @param port The port on which to listen to oncoming connections
 */
void Server::listen(const int port) {
	try {
		boost::asio::io_service io_service;

		tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));

		for (;;) {
			tcp::socket socket(io_service);
			acceptor.accept(socket); // Wait for connection

			// Retrieve message client sent, if any
			boost::array<char, 128> buf;
			boost::system::error_code error;
			socket.read_some(boost::asio::buffer(buf), error);

			// Parse the pattern and log number the client sent us
			std::vector<std::string> splits;
			std::string data = boost::lexical_cast<std::string>(buf.data());
			boost::split(splits, data, boost::is_any_of("::::"),
					boost::token_compress_on);
			std::string pattern = splits[0];
			std::string machine_num = splits[1];
			std::string log_name = "machine."
					+ boost::lexical_cast<std::string>(machine_num[0]) + ".log";

			// Create logfile, save
			LogCreator log_file;
			log_file.create_log(log_name);

			// Run grep on the logfile, then stream the results back to the client
			log_file.grep(pattern, log_name, socket);
		}
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
