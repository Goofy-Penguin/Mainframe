#pragma once

#include <memory>
#include <thread>
#include <mainframe/networking/socket.h>
#include <mainframe/utils/event.hpp>

namespace mainframe {
	namespace ptalk {
		class Client;

		class Server {
			networking::Socket sock;
			std::thread* threadListener = nullptr;
			bool listening = false;

		public:
			utils::Event<std::shared_ptr<Client>> onClient;

			Server() = default;
			~Server();

			bool listen(int port);
			void stop();
		};
	}
}