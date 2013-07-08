#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <string>
#include <stdio.h>
#include "Querier.h"
#include "Constants.h"
#include <map>
#include "UnitTests.h"
using boost::asio::ip::tcp;

/*
 * Main entry point to the client executable.
 */
int main(int argc, char* argv[])
{
	try
	{
		// Get input from the user
		// (which host(s) we should query and the pattern)
		std::vector<int> hosts_to_query;
		if (argc < 3)
		{
			std::cerr
					<< "Usage: <\"pattern to search for\"> <machine.1.log> <machine.2.log> ... <machine.n.log>"
					<< std::endl;
			return 1;
		}

		// Parse argument(s) to find which host(s) to query
		// Host(s) to query are resolved by mapping i to an address
		// e.g., machine.1.log == "linux7.ews.illinois.edu"
		else
		{
			boost::regex re1("machine[.][0-9]+[.]log");
			boost::regex re2("[0-9]+");
			for (int i = 1; i < argc; i++)
			{
				std::string s(argv[i]);
				if (boost::regex_search(s, re1))
				{
					boost::smatch match;
					boost::regex_search(s, match, re2);
					std::string i(match[0]);
					hosts_to_query.push_back(boost::lexical_cast<int>(i));
				}
			}

			// Attempt to query the host(s), if any
			std::string grep_pattern = argv[1];
			boost::thread_group tgroup;
			std::map<int, std::string> host_map = Constants::get_host_map();
			boost::asio::io_service io_service;

			// Query hosts
			for (int i = 0; i < hosts_to_query.size(); i++)
			{
				Querier querier;
				querier.query_logfile(grep_pattern, host_map[hosts_to_query[i]],
						Constants::get_service_name(),
						boost::lexical_cast<std::string>(hosts_to_query[i]),
						boost::ref(io_service));
			}

			// Print the content(s) of the log(s) to the terminal
			for (int i = 0; i < hosts_to_query.size(); i++)
			{
				std::string num = boost::lexical_cast<std::string>(
						hosts_to_query[i]);
				std::string log_name = "machine." + num + ".log";
				std::ifstream log;
				log.open(log_name.c_str());
				// File exists
				if (log.is_open())
				{
					std::cout << log_name + "content:" << std::endl;
					std::string line;
					while (!log.eof())
					{
						std::getline(log, line);
						line += "\n";
						std::cout << line; // << std::endl;
					}
				}
				else // File !exists
				{
					std::cout << log_name + " not retrieved!" << std::endl;
				};
			}

			/*
			 * Unit test 1
			 */
			if (Constants::allow_unit_tests())
			{
				std::cout
						<< "Diffing logs returned from server(s) with local generated one using same log generation method"
						<< std::endl;
				std::cout
						<< "Output of line #'s after this line means the test failed (however, a single line of numbers is acceptable, "
								"because we cannot detect the return char from the boost library using the system diff command."
						<< std::endl;
				UnitTests::UnitTest1();
			}

			// Cleanup local log files
			for (int i = 0; i < hosts_to_query.size(); i++)
			{
				std::string num = boost::lexical_cast<std::string>(
						hosts_to_query[i]);
				std::string log_name = "machine." + num + ".log";
				remove(log_name.c_str());
			}
		}
	} catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
