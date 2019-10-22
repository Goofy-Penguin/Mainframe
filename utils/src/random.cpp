#include <mainframe/utils/random.h>
#include <stdlib.h>
#include <ctime>

namespace mainframe {
	namespace utils {
		namespace random {
			void init() {
				static bool did = false;
				if (did) return;

				did = true;
				srand(static_cast<unsigned int>(std::time(nullptr)));
			}

			int get(int min, int max) {
				init();
				return rand() % (max - min) + min;
			}
		}
	}
}