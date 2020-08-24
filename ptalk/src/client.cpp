#include <mainframe/networking/packet.h>
#include <mainframe/ptalk/server.h>
#include <mainframe/ptalk/client.h>
#include <nlohmann/json.hpp>

namespace mainframe {
	namespace ptalk {
		Client::~Client() {
			disconnect();
			cleanupThreads();
		}

		networking::SocketError Client::connect(const std::string& host, int port) {
			// thread safty for callbacks calling connect at the same time
			std::lock_guard<std::mutex> guard(lock);
			if (connecting) return networking::SocketError::invalidSocket;
			connecting = true;

			// force clean socket
			sock.close();

			if (!sock.create()) return networking::SocketError::invalidSocket;
			auto conret = sock.connect(host, port);
			connecting = false;

			if (conret != networking::SocketError::success) {
				onConnectFailed(conret);
				return conret;
			}

			startThreads();
			onConnect();

			return networking::SocketError::success;
		}

		void Client::startThreads() {
			cleanupThreads();
			threadSend = new std::thread([this]() { sendLoop(); });
			threadRecv = new std::thread([this]() { recvLoop(); });
		}

		void Client::disconnect() {
			sock.close();
		}

		void Client::cleanupThread(std::thread*& thread) {
			if (thread != nullptr) {
				if (thread->joinable()) thread->join();

				delete thread;
				thread = nullptr;
			}
		}

		void Client::cleanupThreads() {
			cleanupThread(threadRecv);
			cleanupThread(threadSend);
		}

		void Client::send(const Message& msg) {
			std::lock_guard<std::mutex> guard(lock);
			toSend.push(msg);
		}

		void Client::send(const Message& msg, OnMessageCallback replyCallback) {
			std::lock_guard<std::mutex> guard(lock);

			toSend.push(msg);
			replyCallbacks[msg.getId()] = replyCallback;
		}

		void Client::respond(const MessageIncoming& msg, const nlohmann::json& data_) {
			Message answer;
			answer.setData(data_);
			answer.setId(msg.getId());

			send(answer);
		}

		void Client::respond(const MessageIncoming& msg, const nlohmann::json& data_, OnMessageCallback replyCallback) {
			Message answer;
			answer.setData(data_);
			answer.setId(msg.getId());

			send(answer, replyCallback);
		}

		void Client::addMethod(const std::string& name, OnMessageCallback callback) {
			std::lock_guard<std::mutex> guard(lock);
			methods[name] = callback;
		}

		void Client::setRef(const std::shared_ptr<Client>& client) {
			ref = client;
		}

		const std::weak_ptr<Client>& Client::getRef() {
			return ref;
		}

		networking::Socket& Client::getSocket() {
			return sock;
		}

		void Client::sendLoop() {
			while (sock.state == networking::Socket::SockState::skCONNECTED) {
				// pop sleeps, but when socket closes we need to stop the loop, so we sleep ourselfs
				if (!toSend.available()) {
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
					continue;
				}

				auto msg = toSend.pop();
				if (!sendMessage(msg)) {
					disconnect();
					return;
				}
			}
		}

		void Client::recvLoop() {
			while (sock.state == networking::Socket::SockState::skCONNECTED) {
				MessageIncoming msg = receiveMessage();
				msg.setClient(getRef().lock());

				OnMessageCallback callback;

				// if name is empty, it's an reply to an earlier sent message
				if (msg.getName().empty()) {
					{
						std::lock_guard<std::mutex> guard(lock);

						auto reply = replyCallbacks.find(msg.getId());
						if (reply == replyCallbacks.end()) {
							disconnect();
							break;
						}

						callback = reply->second;
						replyCallbacks.erase(reply);
					}

					callback(msg);
					continue;
				}

				onMessage(msg);
				if (msg.isCancelled()) continue;

				{
					std::lock_guard<std::mutex> guard(lock);

					auto method = methods.find(msg.getName());
					if (method == methods.end()) continue;

					callback = method->second;
				}

				callback(msg);
			}
		}

		MessageIncoming Client::receiveMessage() {
			MessageIncoming ret;
			networking::Packet buff;

			// find out how big the message is
			buff.resize(sizeof(uint32_t));
			if (!sock.receiveAll(buff.data(), static_cast<int>(buff.size())))
				return ret;

			buff.seek(buff.begin());
			auto messagesize = buff.read<uint32_t>();

			// read the data
			buff.seek(buff.begin());
			buff.resize(messagesize);
			if (!sock.receiveAll(buff.data(), static_cast<int>(buff.size())))
				return ret;

			buff.setlengthFormat(mainframe::networking::LengthType::UInt16);
			ret.setId(buff.read<uint32_t>());
			ret.setName(buff.read<std::string>());

			buff.setlengthFormat(mainframe::networking::LengthType::UInt32);
			ret.setData(nlohmann::json::from_bson(buff.read<std::vector<uint8_t>>()));

			return ret;
		}

		bool Client::sendMessage(const Message& msg) {
			networking::Packet buff;

			buff.setlengthFormat(mainframe::networking::LengthType::UInt16);
			buff.write(msg.getId());
			buff.write(msg.getName());

			buff.setlengthFormat(mainframe::networking::LengthType::UInt32);
			buff.write(nlohmann::json::to_bson(msg.getData()));

			// seek to begin to insert length in front
			buff.seek(buff.begin());
			buff.write(static_cast<uint32_t>(buff.size()));

			return sock.sendAll(buff.data(), static_cast<int>(buff.size()));
		}
	}
}