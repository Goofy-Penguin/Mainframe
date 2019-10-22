#include <mainframe/utils/string.h>
#include <sstream>

namespace mainframe {
	namespace utils {
		namespace string {
			std::vector<std::string> split(const std::string& haystack, char needle) {
				std::vector<std::string> elems;
				std::stringstream ss(haystack);
				std::string item;

				while (std::getline(ss, item, needle)) {
					elems.push_back(item);
				}

				return elems;
			}

			std::vector<std::string> split(const std::string& haystack, const std::string& needle) {
				std::vector<std::string> ret;

				size_t oldpos = 0;
				while (true) {
					auto nextpos = haystack.find(needle, oldpos);
					if (nextpos == std::string::npos) break;

					ret.emplace_back(haystack.begin() + oldpos, haystack.begin() + nextpos);
					nextpos += needle.size();
					oldpos = nextpos;
				}

				return ret;
			}

			std::string replace(const std::string& haystack, const std::string& needle, const std::string& replacement) {
				if (needle.empty()) return {};
				std::string ret(haystack);

				size_t start_pos = 0;
				while ((start_pos = ret.find(needle, start_pos)) != std::string::npos) {
					ret.replace(start_pos, needle.length(), replacement);
					start_pos += replacement.length();
				}

				return ret;
			}
		}
	}
}