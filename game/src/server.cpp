#include <mainframe/game/server.h>
#include <mainframe/game/player.h>

namespace mainframe {
	namespace game {
		void Server::init() {
			Engine::init();

			if (!socket.create()) throw std::runtime_error("failed to create socket");
			if (!socket.bind(5376)) throw std::runtime_error("failed to bind socket");
			if (!socket.listen()) throw std::runtime_error("failed to listen socket");

			listenerThread = new std::thread([this]() {
				while (!shuttingdown) {
					auto sock = std::make_unique<networking::Socket>();
					if (!socket.accept(sock.get())) {
						continue;
					}

					registerClient(std::move(sock));
				}
			});
		}

		void Server::registerClient(std::unique_ptr<networking::Socket> socket) {
			addPlayer(std::make_shared<Player>(this, std::move(socket)));
		}

		void Server::addPlayer(const std::shared_ptr<Player>& ply) {
			playerLock.lock();
			players.push_back(ply);
			playerLock.unlock();
		}

		void Server::tick() {
			Engine::tick();

			playerLock.lock();
			auto playersCopy = players;
			playerLock.unlock();
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
			shuttingdown = true;
			socket.close();
		}
	}
}
