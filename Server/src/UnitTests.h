#ifndef UNITTESTS_H_
#define UNITTESTS_H_

#include <cstdlib>
#include <fstream>

class UnitTests
{
public:
	/*
	 * Unit Test #1. Generate the same log file that would be generated on one of the servers.
	 * Grep this log file after creation and compare it with what results that the servers return.
	 *
	 */
	static void UnitTest1()
	{
		int size = Constants::get_file_size();
		std::string pattern = "TargetL";

		int line = 1, b_size = 0, count = 10;
		size *= (1024 * 1024);
		char *buffer = new char[(1024 * 256) + 1];

		sprintf(buffer, "TargetL\n");

		while (b_size < (1024 * 256))
		{ //creates the buffer we write to log
			if (line % count == 0)
			{
				strcat(buffer, "TargetL\n");
				b_size += 8;
			}
			else
			{
				strcat(buffer, "Garbage\n");
				b_size += 8;
			}

			line++;
		}

		FILE *log;
		std::string logname = "UnitTest1.log";
		log = fopen(logname.c_str(), "w");

		while (size > 0)
		{ //writes to disk in chunks to avoid overhead
			size -= b_size;

			fwrite(buffer, sizeof(char), 1024 * 256, log);
		}

		delete buffer;
		fclose(log);

		std::string grep_cmd = "grep -n " + pattern + " " + "UnitTest1.log "
				+ " >> " + "UnitTestGrepResults.log";
		system(grep_cmd.c_str());
		std::string grep_log = "UnitTestGrepResults.log";
		std::string diff1 = "diff machine.1.log UnitTestGrepResults.log";
		std::string diff2 = "diff machine.2.log UnitTestGrepResults.log";
		std::string diff3 = "diff machine.3.log UnitTestGrepResults.log";
		system(diff1.c_str());
		system(diff2.c_str());
		system(diff3.c_str());
		remove(logname.c_str());
		remove(grep_log.c_str());
	}
};

#endif /* UNITTESTS_H_ */
