#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <mainframe/numbers/pi.h>

namespace mainframe {
	namespace math {
		class Angle {
		public:
			template<class T>
			static T toRadians(T degrees) {
				return degrees * static_cast<T>(numbers::pi<T> / 180);
			}

			template<class T>
			static T toDegrees(T radians) {
				return radians * static_cast<T>(180 / numbers::pi<T>);
			}
		};
	}
}
