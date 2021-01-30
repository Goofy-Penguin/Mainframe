#pragma once

#include <mainframe/game/engine.h>
#include <mainframe/game/messageincomming.h>
#include <mainframe/game/messageoutgoing.h>
#include <mainframe/networking/socket.h>
#include <mainframe/utils/ringbuffer.hpp>
#include <thread>

namespace mainframe {
	namespace game {
		class Server;

		class Player {
			bool disconnected = false;
			Server& server;
			std::unique_ptr<networking::Socket> socket;

			std::thread* receiver;
			std::thread* sender;

			utils::ringbuffer<std::shared_ptr<MessageIncomming>> incomming {128};
			utils::ringbuffer<std::shared_ptr<MessageOutgoing>> outgoing {128};

		public:
			Player(Server& server_, std::unique_ptr<networking::Socket> socket_);
			~Player();

			void disconnect();

			void tick();
			void send(const MessageOutgoing& message);

			virtual std::shared_ptr<MessageIncomming> createIncommingMessage(uint32_t id);
		};
	}
}
