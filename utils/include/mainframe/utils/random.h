#pragma once

namespace mainframe {
	namespace utils {
		class random {
			static void init();

		public:
			static int get(int min, int max);
		};
	}
}