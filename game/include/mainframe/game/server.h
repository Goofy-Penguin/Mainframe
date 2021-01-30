#pragma once

#include <mainframe/game/engine.h>
#include <mainframe/networking/socket.h>
#include <vector>
#include <memory>

namespace mainframe {
	namespace game {
		class Player;

		class Server : public Engine {
			std::vector<std::shared_ptr<Player>> players;
			networking::Socket socket;

		public:
			virtual void tick() override;
			virtual void init() override;
			virtual void run() override;

			void shutdown();
			~Server();
		};
	}
}
