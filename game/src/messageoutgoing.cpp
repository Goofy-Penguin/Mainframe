#include <mainframe/game/messageoutgoing.h>

namespace mainframe {
	namespace game {
		void MessageOutgoing::construct() {
			constructed = true;
		}

		bool MessageOutgoing::isConstructed() {
			return constructed;
		}
	}
}