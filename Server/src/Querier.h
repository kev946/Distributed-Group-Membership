#ifndef QUERIER_H_
#define QUERIER_H_

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <fstream>
#include <map>

/*
 * Querier class sends and receives messages
 * from a server
 */
class Querier
{

public:
	Querier();
	virtual ~Querier();
	void query_logfile(const std::string& grep_pattern,
			const std::string& host_name, const std::string& service,
			const std::string& log_number, boost::asio::io_service& io_service);
};

#endif /* QUERIER_H_ */
