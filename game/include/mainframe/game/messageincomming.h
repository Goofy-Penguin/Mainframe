#pragma once

#include <mainframe/networking/socket.h>
#include <mainframe/networking/packet.h>

namespace mainframe {
	namespace game {
		class Player;

		class MessageIncomming : public networking::Packet {
		public:
			virtual void parse();
			virtual void process(Player* player);
		};
	}
}
