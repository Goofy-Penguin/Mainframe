#include <mainframe/database/base/result.h>

namespace mainframe {
	namespace database {
		Result::Result(bool success) : failed(!success) {

		}
		bool Result::success() {
			return !failed;
		}
	}
}