/*
 * Server.cpp
 *
 *  Created on: Jun 29, 2013
 *      Author: klpeter2
 */

#include "Server.h"

using boost::asio::ip::udp;

/*
 * Construct the server.
 * Running initially set to false.
 *
 */
Server::Server(boost::asio::io_service& ioService_, const std::string& ip) :
		socket(ioService_, udp::endpoint(udp::v4(), 55555)), myIp(ip)
{
	running = false;
	myPort = 55555;
	std::string logName = myIp + ".log";
	log.open(logName.c_str());
	time_t time = std::time(NULL);
	log << "(ip, timestamp, port) Created at " << time << std::endl;
}

Server::~Server()
{

}

/*
 * Start the server.
 * Ping the introducer process as well.
 *
 */
void Server::start()
{
	if (!running)
	{
		running = true;
		startReceive();
	}

	std::string message = messageCreator.message(2, myIp, myPort);
	boost::thread serverThread = boost::thread(boost::bind(&Server::ping, this, introducerIp /* introducer process */, message));
	boost::thread pingThread = boost::thread(boost::bind(&Server::pingServers, this));
	boost::thread leaveThread = boost::thread(boost::bind(&Server::checkForLeaves, this));
}

/*
 * Listen in the background for a new request (ping).
 * If received, the io_service object invokes the handleReceive method.
 *
 */
void Server::startReceive()
{
	try
	{
		socket.async_receive_from(boost::asio::buffer(recvBuf), remoteEndpoint,
				boost::bind(&Server::handleReceive, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));

	} catch (std::exception& e)
	{
		running = false;
		std::cerr << e.what() << std::endl;
	}
}

/*
 * Invoked when a message is received from a process (callback method).
 * *** This is where we decide what to do with the message received ***
 *
 * @param error Indicates whether the operation succeeded or failed
 * @param bytesTransferred Number of bytes received
 */
void Server::handleReceive(const boost::system::error_code& error,
		std::size_t bytesTransferred)
{
	// Parse message we received
	std::string message(recvBuf.begin(), recvBuf.end());
	std::vector<std::string> result;
	boost::split(result, message, boost::is_any_of("_"));
	int action = boost::lexical_cast<int>(result[0]);
	std::string ip = result[1];
	time_t timestamp = boost::lexical_cast<time_t>(result[2]);
	int port = boost::lexical_cast<int>(result[3]);
	MembershipList::tuple tuple(ip, timestamp, port);

	std::string msg;
	switch(action)
	{
	case 0: // Received a ping, send an ack
		msg = messageCreator.message(1, myIp, myPort);

		// New member joined
		if(!membershipList.isInList(ip))
		{
			membershipList.add(tuple);
			std::string s = membershipList.tupleToString(tuple);
			log << s << " added to membership list." << std::endl;
		}
		ping(ip, msg);
		break;
	case 1: // Received an ack. Update membership list timestamp
		membershipList.update(tuple);
		break;
	case 2: // JOIN
		addToMembershipList(tuple);
		msg = messageCreator.message(membershipList.get(), myIp, myPort);
		ping(ip, msg); // Send back membership list
		break;
	case 3: // LIST (list being received)
		if(ip == myIp)
			break;
		else
		{
			// Update our membership list
			listCaseHelper(message);
			break;
		}
	default:
		break;
	}
	startReceive();
}

void Server::ping(const std::string& ip, const std::string& message)
{
	try
	{
		boost::asio::io_service io_service;

		udp::resolver resolver(io_service);
		udp::resolver::query query(udp::v4(), ip, "55555");
		udp::endpoint receiver_endpoint = *resolver.resolve(query);

		udp::socket socket(io_service);
		socket.open(udp::v4());

		socket.send_to(boost::asio::buffer(message), receiver_endpoint);

	} catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

}

void Server::stop()
{
	running = false;
}

void Server::addToMembershipList(MembershipList::tuple& tuple)
{
	membershipList.add(tuple);
	std::string s = membershipList.tupleToString(tuple);
	log << s << " added to membership list." << std::endl;
}

void Server::listCaseHelper(std::string& message)
{
	// Parse message and construct tuples.
	// Add tuples to the membership list.
	std::vector<std::string> results1;
	//boost::split(results, message, "__(", boost::token_compress_on);
	boost::algorithm::split_regex(results1, message, boost::regex("[(]"));
	for(int i = 1; i < results1.size(); ++i)
	{
		std::vector<std::string> results2;
		boost::split(results2, results1[i], boost::is_any_of(", "), boost::token_compress_on);
		std::string ip = results2[0];
		time_t timestamp = boost::lexical_cast<time_t>(results2[1]);
		int port = 55555;
		MembershipList::tuple tuple(ip, timestamp, port);
		membershipList.add(tuple);
		std::string s = membershipList.tupleToString(tuple);
		log << s << " added to membership list." << std::endl;
	}
}

/*
 * Ping the members of the membership list
 * every once in a while to make sure they're still
 * alive. Remove members that crash or don't respond.
 *
 */
void Server::pingServers()
{
	time_t time = std::time(NULL);
	std::vector<MembershipList::tuple> list = membershipList.get();
	while(true)
	{
		// Ping every second
		time_t diff = std::time(NULL) - time;
		if(diff > 1)
		{
			list = membershipList.get();
			time = std::time(NULL);
		}
		if(membershipList.size() > 1 && diff > 1)
		{
			// Ping the members to see if still alive
			std::string message = messageCreator.message(0, myIp, myPort);
			for(std::vector<MembershipList::tuple>::iterator it = list.begin(); it != list.end(); ++it)
			{
				std::string ip = it->get<0>();
				if(ip.compare(myIp) != 0)
				{
					ping(ip, message);
				}
			}
		}
	}
}

/*
 * Checks the membership list for failed processes.
 *
 */
void Server::checkForLeaves()
{
	std::vector<MembershipList::tuple> list;
	time_t time = std::time(NULL);
	while(true)
	{
		time_t diff = std::time(NULL) - time;
		if(diff >= 3) // allow for the 2 second ping-ack round-trip time
		{
			list = membershipList.get();
			for(std::vector<MembershipList::tuple>::iterator it = list.begin(); it != list.end(); ++it)
			{
				std::string ip = it->get<0>();
				if(ip.compare(myIp) != 0)
				{
					time_t mbrTime = it->get<1>();
					if((time - mbrTime) > 3)
					{
						std::string s = membershipList.tupleToString(*it);
						membershipList.remove(ip);
						std::cout << s << " timed out. Dropped from membership list." << std::endl;
						log << s << " timed out. Dropped from membership list." << std::endl;
					}
				}
			}
			time = std::time(NULL);
		}
	}
}

void Server::setIntroducerIp(const std::string& ip)
{
	introducerIp = ip;
}

std::string Server::getIntroducerIp()
{
	return introducerIp;
}
