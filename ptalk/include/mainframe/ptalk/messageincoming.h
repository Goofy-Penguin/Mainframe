#pragma once

#include <memory>
#include <mainframe/ptalk/message.h>

namespace mainframe {
	namespace ptalk {
		class Client;
		class MessageIncoming;

		using OnMessageCallback = mainframe::utils::Event<MessageIncoming&>::Func;

		class MessageIncoming : public Message {
			std::shared_ptr<Client> client;

		public:
			MessageIncoming() = default;
			MessageIncoming(std::shared_ptr<Client> client_, int id_, const std::string& name_, const nlohmann::json& data_);

			void setClient(const std::shared_ptr<Client>& client_);
			const std::shared_ptr<Client>& getClient() const;

			void respond(const nlohmann::json& data);
			void respond(const nlohmann::json& data, OnMessageCallback replyCallback);
		};
	}
}