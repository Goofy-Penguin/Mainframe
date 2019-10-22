#pragma once

#include <vector>
#include <string>

namespace mainframe {
	namespace utils {
		namespace string {
			std::vector<std::string> split(const std::string& haystack, char needle);
			std::vector<std::string> split(const std::string& haystack, const std::string& needle);
			std::string replace(const std::string& haystack, const std::string& needle, const std::string& replacement);
		};
	}
}