#pragma once

#include <mainframe/game/engine.h>
#include <mainframe/networking/socket.h>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>

namespace mainframe {
	namespace game {
		class Player;

		class Server : public Engine {
			std::vector<std::shared_ptr<Player>> players;
			networking::Socket socket;
			std::thread* listenerThread = nullptr;
			bool shuttingdown = false;
			std::mutex playerLock;

		public:
			virtual void update(float deltaTime) override;
			virtual void init() override;
			virtual void run() override;
			virtual void quit() override;

			virtual void registerClient(std::unique_ptr<networking::Socket> socket);

			void addPlayer(const std::shared_ptr<Player>& ply);

			template<class T = Player>
			std::vector<std::shared_ptr<T>> getPlayers() {
				playerLock.lock();
				std::vector<std::shared_ptr<T>> ret;
				for (auto& ply : players) {
					ret.push_back(std::dynamic_pointer_cast<T>(ply));
				}

				playerLock.unlock();

				return ret;
			}

			~Server();
		};
	}
}
