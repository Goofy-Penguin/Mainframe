#include <mainframe/game/messageincomming.h>

namespace mainframe {
	namespace game {
		void MessageIncomming::setBuffer(std::vector<uint8_t> buff) {
			packet.getBuffer() = buff;
		}

		mainframe::networking::Packet& MessageIncomming::getReader() {
			return packet;
		}
	}
}