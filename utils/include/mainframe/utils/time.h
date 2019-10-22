#pragma once

#include <string>

namespace mainframe {
	namespace utils {
		namespace time {
			enum class Duration {
				NEVER, NON_ZERO, ALWAYS
			};

			float getMS();
			void resetTime();

			std::string getDurationFormated(time_t seconds, Duration secondMode = Duration::NON_ZERO, Duration minuteMode = Duration::NON_ZERO, Duration hourMode = Duration::NON_ZERO, Duration dayMode = Duration::NON_ZERO, Duration weekMode = Duration::NON_ZERO, Duration monthMode = Duration::NON_ZERO, Duration yearMode = Duration::NON_ZERO);
			std::string getDateFormated(time_t unixtime, const std::string& format);
		};
	}
}