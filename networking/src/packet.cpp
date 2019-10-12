#include <mainframe/networking/packet.h>

namespace mainframe {
	namespace networking {
		bool Packet::seek(size_t offset) {
			if (offset > end()) return false;

			pos = offset;
			return true;
		}
	}
}