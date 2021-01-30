#include <mainframe/game/server.h>
#include <mainframe/game/player.h>

namespace mainframe {
	namespace game {
		void Server::init() {
			Engine::init();

			if (!socket.create()) throw std::runtime_error("failed to create socket");
			if (!socket.bind(5376)) throw std::runtime_error("failed to bind socket");
			if (!socket.listen()) throw std::runtime_error("failed to listen socket");
		}

		void Server::tick() {
			Engine::tick();

			auto playersCopy = players;
			for (auto& ply : playersCopy) {
				ply->tick();
			}
		}

		void Server::run() {
			Engine::run();
		}

		Server::~Server() {
			shutdown();
		}

		void Server::shutdown() {
			socket.close();
		}
	}
}
