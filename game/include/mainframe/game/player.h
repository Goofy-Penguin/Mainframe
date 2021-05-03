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
			Server* server = nullptr;
			std::unique_ptr<networking::Socket> socket;

			std::thread* receiver = nullptr;
			std::thread* sender = nullptr;

			utils::ringbuffer<std::shared_ptr<MessageIncomming>> incomming {4096};
			utils::ringbuffer<std::shared_ptr<MessageOutgoing>> outgoing {4096};

			std::string name;

		public:
			Player() = default;
			Player(std::unique_ptr<networking::Socket> socket_);
			Player(Server* server_, std::unique_ptr<networking::Socket> socket_);
			virtual ~Player();

			Server* getServer() const;
			networking::Socket& getSocket();
			void setSocket(std::unique_ptr<networking::Socket> socket_);
			void joinAndCleanThreads();

			const std::string& getName();
			void setName(const std::string& name_);

			virtual void disconnect();

			virtual void tick();
			virtual void send(std::shared_ptr<MessageOutgoing> message);

			virtual std::shared_ptr<MessageIncomming> createIncommingMessage(uint32_t id);
			virtual bool onMessage(MessageIncomming* message);
		};
	}
}
