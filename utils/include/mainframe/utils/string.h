#pragma once

#include <vector>
#include <string>

namespace mainframe {
	namespace utils {
		namespace string {
			std::vector<std::string> split(const std::string& haystack, char needle);
			std::vector<std::string> split(const std::string& haystack, const std::string& needle);
			std::string replace(const std::string& haystack, const std::string& needle, const std::string& replacement);

            // https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
            // trim from start (in place)
            static void ltrim(std::string& s);

            // trim from end (in place)
            static void rtrim(std::string& s);

            // trim from both ends (in place)
            static void trim(std::string& s);
            // trim from start (copying)
            static std::string ltrim_copy(std::string s);

            // trim from end (copying)
            static std::string rtrim_copy(std::string s);

            // trim from both ends (copying)
            static std::string trim_copy(std::string s);
		};
	}
}