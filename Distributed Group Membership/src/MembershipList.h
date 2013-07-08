/*
 * MembershipList.h
 *
 *  Created on: Jun 30, 2013
 *      Author: klpeter2
 */

#ifndef MEMBERSHIPLIST_H_
#define MEMBERSHIPLIST_H_

#include <vector>
#include <boost/tuple/tuple.hpp>
#include <time.h>
#include <string>
#include <boost/lexical_cast.hpp>
#include <iostream>

class MembershipList
{
public:

	typedef boost::tuple<std::string, time_t, int> tuple;

	MembershipList();
	virtual ~MembershipList();
	void add(tuple tup);
	void remove(const std::string& ip);
	bool isInList(const std::string& ip);
	std::vector<tuple> getList();
	void print();
	std::vector<tuple> get();
	std::string tupleToString(const MembershipList::tuple& tup);
	void update(const MembershipList::tuple& tup);
	int size();

private:

	std::vector<tuple> membershipList;
};

#endif /* MEMBERSHIPLIST_H_ */
