#ifndef LOGCREATOR_H_
#define LOGCREATOR_H_

#include <string.h>
#include <string>
#include <fstream>
#include <stdio.h>
#include <cstdlib>
#include <boost/asio.hpp>
#include "Constants.h"
using boost::asio::ip::tcp;

class LogCreator
{
public:
	LogCreator();
	virtual ~LogCreator();
	void create_log(const std::string& log_name);
	void grep(const std::string& pattern, const std::string& log_name, tcp::socket& socket);
private:
	FILE *log;
	void gen_lines(int count, long size);
};

#endif /* LOGCREATOR_H_ */
