/*
 * MessageCreator.cpp
 *
 *  Created on: Jun 30, 2013
 *      Author: klpeter2
 */

#include "MessageCreator.h"

MessageCreator::MessageCreator()
{
	// TODO Auto-generated constructor stub

}

MessageCreator::~MessageCreator()
{
	// TODO Auto-generated destructor stub
}

/*
 * Creates a message with a timestamp.
 *
 * @param type The action to have the receiver of the message take
 * @param myIp IP address of the sender
 * @param myPort Port of the sender
 */
std::string MessageCreator::message(const int type, const std::string& myIp,
		const int myPort)
{
	std::string timestamp = boost::lexical_cast<std::string>(std::time(NULL));
	std::string port = boost::lexical_cast<std::string>(myPort);
	std::string tmpmsg = myIp + "_" + timestamp + "_" + port + "_";
	std::string msg;

	switch (type)
	{
	case 0: // PING
		msg = "0_" + tmpmsg;
		return msg;
	case 1: // ACK
		msg = "1_" + tmpmsg;
		return msg;
	case 2: // JOIN
		msg = "2_" + tmpmsg;
		return msg;
	default:
		return "";
	}
}

std::string MessageCreator::message(
		const std::vector<MembershipList::tuple>& memembershipList,
		const std::string& myIp, const int myPort)
{
	std::string timestamp = boost::lexical_cast<std::string>(std::time(NULL));
	std::string port = boost::lexical_cast<std::string>(myPort);
	std::string tmpmsg = myIp + "_" + timestamp + "_" + port + "_";
	std::string msg = "3_" + tmpmsg;

	for (std::vector<MembershipList::tuple>::const_iterator it =
			memembershipList.begin(); it != memembershipList.end(); ++it)
	{
		std::string ip = it->get<0>();
		std::string timestamp = boost::lexical_cast<std::string>(it->get<1>());
		std::string id = boost::lexical_cast<std::string>(it->get<2>());
		std::string tuple = "_(" + ip + ", " + timestamp + ", " + id + ")_";
		msg += tuple;
	}
	return msg;
}

