#include <mainframe/utils/string.h>
#include <sstream>

namespace mainframe {
	namespace utils {
		std::vector<std::string> string::split(const std::string& haystack, char needle) {
			std::vector<std::string> elems;
			std::stringstream ss(haystack);
			std::string item;

			while (std::getline(ss, item, needle)) {
				elems.push_back(item);
			}

			return elems;
		}

		std::vector<std::string> string::split(const std::string& haystack, const std::string& needle) {
			std::vector<std::string> ret;

			auto oldpos = 0;
			while (true) {
				auto nextpos = haystack.find(needle, oldpos);
				if (nextpos == std::string::npos) break;

				ret.emplace_back(haystack.begin() + oldpos, haystack.begin() + nextpos);
				nextpos += needle.size();
				oldpos = nextpos;
			}

			return ret;
		}
	}
}