#include "Querier.h"
using boost::asio::ip::tcp;

Querier::Querier()
{

}

Querier::~Querier()
{

}

void Querier::query_logfile(const std::string& grep_pattern,
		const std::string& host_name, const std::string& service,
		const std::string& log_number, boost::asio::io_service& io_service)
{
	tcp::resolver resolver(io_service);
	tcp::resolver::query query(host_name, service);

	// Get a list of possible tcp endpoints
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	tcp::resolver::iterator end;

	tcp::socket socket(io_service);
	boost::system::error_code error = boost::asio::error::host_not_found;

	// Try to connect to one of the endpoints
	while (error && endpoint_iterator != end)
	{
		socket.close();
		socket.connect(*endpoint_iterator++, error);
	}
	if (error)
	{
		boost::system::system_error e(error);
		std::cerr << e.what() << std::endl;
		return;
	}

	// Send message to the host
	std::string message = grep_pattern + "::::" + log_number;
	boost::system::error_code ignored_error;
	boost::asio::write(socket, boost::asio::buffer(message),
			boost::asio::transfer_all(), ignored_error);

	// Listen for the host's response and write it to temporary file
	std::ofstream grep_log;
	std::string log_name = "machine." + log_number + ".log";
	grep_log.open(log_name.c_str());

	for (;;)
	{
		char buf[128];
		boost::system::error_code error;

		size_t len = socket.read_some(boost::asio::buffer(buf), error);

		if (error == boost::asio::error::eof)
			break; // Connection closed cleanly by peer.
		else if (error)
		{
			boost::system::system_error e(error);
			std::cerr << e.what() << std::endl;
			return;
		}

		grep_log << buf << std::endl;
	}

	grep_log.close();
}

