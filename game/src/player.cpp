#include <mainframe/game/player.h>
#include <fmt/printf.h>

namespace mainframe {
	namespace game {
		Player::Player(std::unique_ptr<networking::Socket> socket_) : Player(nullptr, std::move(socket_)) {

		}

		Player::Player(Server* server_, std::unique_ptr<networking::Socket> socket_) : server(server_) {
			setSocket(std::move(socket_));
		}

		const std::string& Player::getName() {
			return name;
		}

		void Player::setName(const std::string& name_) {
			name = name_;
		}

		void Player::setSocket(std::unique_ptr<networking::Socket> socket_) {
			socket = std::move(socket_);
			if (socket == nullptr) return;

			sender = new std::thread([this]() {
				while (!disconnected) {
					while (outgoing.available()) {
						auto message = outgoing.pop();
						if (!socket->sendAll(message->data(), message->size())) {
							disconnect();
						}
					}

					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
			});

			receiver = new std::thread([this]() {
				while (!disconnected) {
					uint32_t msglen = 0;
					if (!socket->receiveAll(reinterpret_cast<unsigned char*>(&msglen), sizeof(msglen))) {
						disconnect();
						break;
					}

					std::vector<unsigned char> buffer;
					buffer.resize(msglen);
					if (!socket->receiveAll(buffer.data(), msglen)) continue;

					uint32_t msgId = *reinterpret_cast<uint32_t*>(&buffer.front());

					auto msg = createIncommingMessage(msgId);
					if (msg == nullptr) continue;

					auto& msgbuff = msg->getReader().getBuffer();
					msgbuff.insert(msgbuff.begin(), buffer.begin() + 4, buffer.end());

					if (!msg->read()) {
						fmt::print("Error reading packet {} from player", msgId);
						continue;
					}

					incomming.push(msg);
				}
			});
		}

		void Player::joinAndCleanThreads() {
			if (sender) {
				if (sender->joinable()) sender->join();
				delete sender;
				sender = nullptr;
			}

			if (receiver) {
				if (receiver->joinable()) receiver->join();
				delete receiver;
				receiver = nullptr;
			}
		}


		networking::Socket& Player::getSocket() {
			return *socket;
		}

		void Player::disconnect() {
			if (disconnected) return;
			disconnected = true;

			if (socket) socket->close();
		}

		Player::~Player() {
			disconnect();
			joinAndCleanThreads();
		}

		bool Player::onMessage(MessageIncomming* message) {
			return true;
		}

		Server* Player::getServer() const {
			return server;
		}

		void Player::send(std::shared_ptr<MessageOutgoing> message) {
			if (!message->isConstructed()) {
				message->construct();

				message->seek(message->begin());
				message->write(static_cast<uint32_t>(message->size()));
			}

			outgoing.push(message);
		}

		void Player::tick() {
			while (incomming.available()) {
				auto msg = incomming.pop();
				msg->setPlayer(this);
				msg->setServer(server);

				if (!onMessage(msg.get())) continue;
				msg->execute();
			}
		}

		std::shared_ptr<MessageIncomming> Player::createIncommingMessage(uint32_t id) {
			throw std::runtime_error("unknown message by id: " + std::to_string(id));
		}
	}
}
