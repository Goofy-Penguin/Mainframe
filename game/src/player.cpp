#include <mainframe/game/player.h>

namespace mainframe {
	namespace game {
		Player::Player(Server& server_, std::unique_ptr<networking::Socket> socket_) : server(server_), socket(std::move(socket_)) {
			sender = new std::thread([this]() {
				while (!disconnected) {
					auto message = outgoing.pop();

					message->construct();
					socket->sendAll(message->getBuffer().data(), message->size());
				}
			});

			receiver = new std::thread([this]() {
				while (!disconnected) {
					uint32_t msglen = 0;
					if (!socket->receiveAll(reinterpret_cast<unsigned char*>(&msglen), sizeof(msglen))) continue;

					std::vector<unsigned char> buffer;
					buffer.resize(msglen);
					if (!socket->receiveAll(buffer.data(), msglen)) continue;

					uint32_t msgId = *reinterpret_cast<uint32_t*>(&buffer.front());

					auto msg = createIncommingMessage(msgId);
					msg->resize(msglen);
					msg->write(buffer.begin() + 4, buffer.end(), false);
					msg->seek(msg->begin());
					msg->parse();

					incomming.push(msg);
				}
			});
		}

		void Player::disconnect() {
			if (disconnected) return;
			disconnected = true;

			socket->close();
		}

		Player::~Player() {
			disconnect();
		}

		void Player::tick() {
			while (incomming.available()) {
				incomming.pop()->process(this);
			}
		}

		std::shared_ptr<MessageIncomming> Player::createIncommingMessage(uint32_t id) {
			throw std::runtime_error("unknown message by id: " + std::to_string(id));
		}
	}
}
