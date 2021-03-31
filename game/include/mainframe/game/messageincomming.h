#pragma once

#include <mainframe/game/world.h>
#include <mainframe/game/server.h>
#include <mainframe/networking/socket.h>
#include <mainframe/networking/packet.h>

namespace mainframe {
	namespace game {
		class MessageIncomming {
			mainframe::networking::Packet packet;
			World* world = nullptr;
			Server* server = nullptr;
			Player* player = nullptr;

		public:
			void setBuffer(std::vector<uint8_t> buff);
			mainframe::networking::Packet& getReader();

			template<class T>
			T* getWorld() {
				return dynamic_cast<T*>(world);
			}

			template<class T>
			T* getServer() {
				return dynamic_cast<T*>(server);
			}

			template<class T>
			T* getPlayer() {
				return dynamic_cast<T*>(player);
			}

			template<class T>
			void setServer(T* serv) {
				server = dynamic_cast<Server*>(serv);
				if (serv != nullptr && server == nullptr) throw std::runtime_error("server dynamic cast failed");
			}

			template<class T>
			void setWorld(T* world_) {
				world = dynamic_cast<World*>(world_);
				if (world_ != nullptr && world == nullptr) throw std::runtime_error("world dynamic cast failed");
			}

			template<class T>
			void setPlayer(T* ply) {
				player = dynamic_cast<Player*>(ply);
				if (ply != nullptr && player == nullptr) throw std::runtime_error("player dynamic cast failed");
			}

			virtual bool read() = 0;
			virtual void execute() = 0;
		};
	}
}
