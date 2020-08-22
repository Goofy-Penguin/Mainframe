#include <mainframe/ptalk/server.h>
#include <mainframe/ptalk/client.h>

namespace mainframe {
	namespace ptalk {
		Server::~Server() {
			close();
		}

		bool Server::listen(int port) {
			if (listening) return false;
			if (!sock.create()) return false;
			if (!sock.bind(port)) return false;
			if (!sock.listen()) return false;

			listening = true;
			threadListener = new std::thread([this]() {
				while (listening) {
					auto client = std::make_shared<Client>();
					if (!sock.accept(&client->getSocket())) continue;

					client->setRef(client);
					client->startThreads();

					onClient(client);
				}
			});

			return true;
		}

		void Server::close() {
			listening = false;
			sock.close();

			if (threadListener != nullptr) {
				if (threadListener->joinable()) threadListener->join();

				delete threadListener;
				threadListener = nullptr;
			}
		}
	}
}