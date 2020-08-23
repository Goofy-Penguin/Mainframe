#pragma once

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <type_traits>
#include <mainframe/networking/socket.h>
#include <mainframe/utils/event.hpp>
#include <mainframe/utils/ringbuffer.hpp>
#include <mainframe/ptalk/messageincoming.h>

namespace mainframe {
	namespace ptalk {
		class Server;

		class Client {
			std::mutex lock;
			std::shared_ptr<Server> server;
			std::weak_ptr<Client> ref;
			networking::Socket sock;
			std::map<std::string, OnMessageCallback> methods;
			std::map<int, OnMessageCallback> replyCallbacks;

			utils::ringbuffer<Message> toSend = {64};

			std::thread* threadRecv = nullptr;
			std::thread* threadSend = nullptr;
			bool connecting = false;

			void sendLoop();
			void recvLoop();

			static void cleanupThread(std::thread*& thread);
			void cleanupThreads();

			MessageIncoming receiveMessage();
			bool sendMessage(const Message& msg);

		public:
			Client() = default;
			~Client();

			utils::Event<> onClose;
			utils::Event<> onConnect;
			utils::Event<networking::SocketError> onConnectFailed;
			utils::Event<MessageIncoming&> onMessage;

			networking::SocketError connect(const std::string& host, int port);
			void disconnect();
			void startThreads();

			void setRef(const std::shared_ptr<Client>& client);
			const std::weak_ptr<Client>& getRef();

			void send(const Message& msg);
			void send(const Message& msg, OnMessageCallback replyCallback);
			void respond(const MessageIncoming& msg, const nlohmann::json& data_);
			void respond(const MessageIncoming& msg, const nlohmann::json& data_, OnMessageCallback replyCallback);

			networking::Socket& getSocket();

			// @param callback: std::function<void(MessageIncomming& message)>
			void addMethod(const std::string& name, OnMessageCallback callback);
		};
	}
}