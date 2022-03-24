#pragma once

#include <mainframe/networking/socket.h>
#include <mainframe/networking/packet.h>

namespace mainframe::game {
	class MessageOutgoing : public networking::Packet {
		bool constructed = false;

	public:
		bool isConstructed();
		virtual void construct();

		virtual ~MessageOutgoing() = default;
	};
}
