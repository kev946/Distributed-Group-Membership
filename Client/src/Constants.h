#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <map>
#include <string>
/*
 * Constants class allows for ease of changing
 * commonly used variables. Make sure to recompile
 * when changing any of the variables in order for
 * them to take effect!
 */
class Constants
{
public:
	/*
	 * Server mappings. Index represents "i"
	 * in machine.i.log.
	 *
	 * E.g. machine.1.log == "linux5.ews.illinois.edu"
	 */
	static std::map<int, std::string> get_host_map()
	{
		std::map<int, std::string> host_map;
		host_map[1] = "linux5.ews.illinois.edu";
		host_map[2] = "linux6.ews.illinois.edu";
		host_map[3] = "linux7.ews.illinois.edu";
		return host_map;
	}

	/*
	 * Protocol under which we query the server(s)
	 * E.g., Query server(s) on port 55555
	 */
	static std::string get_service_name()
	{
		return "55555";
	}

	/*
	 * Path of the temporary directory
	 */
	static std::string get_temp_dir_path()
	{
		return "/tmp/";
	}

	/*
	 * Port on which the server should listen
	 */
	static int get_port()
	{
		return 55555;
	}

	/*
	 * Log file size in megabytes (MB)
	 */
	static int get_file_size()
	{
		return 10;
	}

	/*
	 * Unit tests enabled by default
	 */
	static bool allow_unit_tests()
	{
		return true;
	}
};

#endif /* CONSTANTS_H_ */
