#include "LogCreator.h"

LogCreator::LogCreator() {

}

LogCreator::~LogCreator() {

}

/*
 * @param log_name What to name the log
 */
void LogCreator::create_log(const std::string& log_name) {
	std::string log_path = Constants::get_temp_dir_path() + log_name;
	log = fopen(log_path.c_str(), "w");
	gen_lines(10, Constants::get_file_size()); // lines, size in MB
	fclose(log);
}

/*
 * @param pattern The pattern to grep for
 * @param log_name The name of the log to grep
 * @param socket The socket in which to send data over
 */
void LogCreator::grep(const std::string& pattern, const std::string& log_name,
		tcp::socket& socket) {
	// absolute paths
	std::string grep_results_path = Constants::get_temp_dir_path() + "grep."
			+ log_name;
	std::string log_path = Constants::get_temp_dir_path() + log_name;

	std::string grep_cmd = "grep -n " + pattern + " " + log_path + " >> "
			+ grep_results_path;
	system(grep_cmd.c_str());

	std::ifstream grep_results;
	grep_results.open(grep_results_path.c_str());

	char buffer[128];
	while (!grep_results.eof()) {
		grep_results.getline(buffer, 128);
		boost::system::error_code ignored_error;
		boost::asio::write(socket, boost::asio::buffer(buffer),
				boost::asio::transfer_all(), ignored_error);
	}

	grep_results.close();

	//cleanup
	std::remove(grep_results_path.c_str());
	std::remove(log_path.c_str());
}

/* This function actually writes the lines to our log file.

 @param count	the count of how often we want our target line to appear
 @param size	the total size of the file
 */
void LogCreator::gen_lines(int count, long size) {
	int line = 1, b_size = 0;
	size *= (1024 * 1024);
	char *buffer = new char[(1024 * 256) + 1];

	sprintf(buffer, "TargetL\n");

	while (b_size < (1024 * 256)) { //creates the buffer we write to log
		if (line % count == 0) {
			strcat(buffer, "TargetL\n");
			b_size += 8;
		} else {
			strcat(buffer, "Garbage\n");
			b_size += 8;
		}

		line++;
	}

	while (size > 0) { //writes to disk in chunks to avoid overhead
		size -= b_size;

		fwrite(buffer, sizeof(char), 1024 * 256, log);
	}

	delete buffer;
}
