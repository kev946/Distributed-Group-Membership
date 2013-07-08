#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <cstdlib>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>
#include "Constants.h"
#include "LogCreator.h"

class Server {
public:
	Server();
	virtual ~Server();
	void listen(const int port);
};

#endif /* SERVER_H_ */
