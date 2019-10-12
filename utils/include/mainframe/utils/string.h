#pragma once

#include <vector>
#include <string>

namespace mainframe {
	namespace utils {
		class string {
		public:
			static std::vector<std::string> split(const std::string& haystack, char needle);
			static std::vector<std::string> split(const std::string& haystack, const std::string& needle);
		};
	}
}