/*
 * MessageCreator.h
 *
 *  Created on: Jun 30, 2013
 *      Author: klpeter2
 */

#ifndef MESSAGECREATOR_H_
#define MESSAGECREATOR_H_

#include <string>
#include <vector>

#include "MembershipList.h"
#include <boost/tuple/tuple.hpp>

class MessageCreator
{
public:
	MessageCreator();
	virtual ~MessageCreator();
	std::string message(const int type, const std::string& myIp,
			const int myPort);
	std::string message(
			const std::vector<MembershipList::tuple>& memembershipList,
			const std::string& myIp, const int myPort);
};

#endif /* MESSAGECREATOR_H_ */
