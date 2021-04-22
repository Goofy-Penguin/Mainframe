#include <mainframe/utils/string.h>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <locale>

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
				if(haystack.empty()) return ret;

				size_t oldpos = 0;
				while (true) {
					auto nextpos = haystack.find(needle, oldpos);
					if (nextpos == std::string::npos) break;

					ret.emplace_back(haystack.begin() + oldpos, haystack.begin() + nextpos);
					nextpos += needle.size();
					oldpos = nextpos;
				}

				if (oldpos != haystack.size()) {
					ret.emplace_back(haystack.begin() + oldpos, haystack.end());
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

			// https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
			// trim from start (in place)
			void ltrim(std::string& s) {
				s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
					return !std::isspace(ch);
				}));
			}

			// trim from end (in place)
			void rtrim(std::string& s) {
				s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
					return !std::isspace(ch);
				}).base(), s.end());
			}

			// trim from both ends (in place)
			void trim(std::string& s) {
				ltrim(s);
				rtrim(s);
			}

			// trim from start (copying)
			std::string ltrim_copy(std::string s) {
				ltrim(s);
				return s;
			}

			// trim from end (copying)
			std::string rtrim_copy(std::string s) {
				rtrim(s);
				return s;
			}

			// trim from both ends (copying)
			std::string trim_copy(std::string s) {
				trim(s);
				return s;
			}
		}
	}
}
