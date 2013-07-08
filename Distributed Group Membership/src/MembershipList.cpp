/*
 * MembershipList.cpp
 *
 *  Created on: Jun 30, 2013
 *      Author: klpeter2
 */

#include "MembershipList.h"

MembershipList::MembershipList()
{

}

MembershipList::~MembershipList()
{

}

/*
 * Add tuple to the membership list.
 */
void MembershipList::add(MembershipList::tuple tup)
{
	if (!isInList(tup.get<0>()))
	{
		membershipList.push_back(tup);
		std::string s = tupleToString(tup);
		std::cout << s << " added to membership list." << std::endl;
	}
}

/*
 * Remove tuple from membership list with matching ip.
 */
void MembershipList::remove(const std::string& ip)
{
	for (std::vector<MembershipList::tuple>::iterator it =
			membershipList.begin(); it < membershipList.end(); it++)
	{
		if (it->get<0>() == ip)
		{
			it = membershipList.erase(it);
		}
	}
}

/*
 * Checks if the ip is in the membership list.
 */
bool MembershipList::isInList(const std::string& ip)
{
	for (std::vector<MembershipList::tuple>::const_iterator it =
			membershipList.begin(); it < membershipList.end(); it++)
	{
		if (it->get<0>() == ip)
		{
			return true;
		}
	}
	return false;
}

std::vector<MembershipList::tuple> MembershipList::getList()
{
	return membershipList;
}

/*
 * Prints the contents of the membership list
 * in 3 tuple format.
 */
void MembershipList::print()
{
	for (std::vector<MembershipList::tuple>::iterator it =
			membershipList.begin(); it < membershipList.end(); it++)
	{
		std::string ip = it->get<0>();
		std::string timestamp = boost::lexical_cast<std::string>(it->get<1>());
		std::string id = boost::lexical_cast<std::string>(it->get<2>());
		std::string tuple = "(" + ip + ", " + timestamp + ", " + id + ")";
		std::cout << tuple << std::endl;
	}
}

std::vector<MembershipList::tuple> MembershipList::get()
{
	return membershipList;
}

std::string MembershipList::tupleToString(const MembershipList::tuple& tup)
{
	std::string ip = tup.get<0>();
	std::string timestamp = boost::lexical_cast<std::string>(tup.get<1>());
	std::string id = boost::lexical_cast<std::string>(tup.get<2>());
	std::string tuple = "(" + ip + ", " + timestamp + ", " + id + ")";
	return tuple;
}

void MembershipList::update(const MembershipList::tuple& tup)
{
	for (std::vector<MembershipList::tuple>::iterator it =
			membershipList.begin(); it < membershipList.end(); it++)
	{
		if(it->get<0>() == tup.get<0>())
		{
			it->get<1>() = tup.get<1>();
		}
	}
}

int MembershipList::size()
{
	return membershipList.size();
}
