#pragma once

#include <memory>
#include <mainframe/ptalk/message.h>

namespace mainframe {
	namespace ptalk {
		class Client;
		class MessageIncomming;

		using OnMessageCallback = mainframe::utils::Event<MessageIncomming&>::Func;

		class MessageIncomming : public Message {
			std::shared_ptr<Client> client;

		public:
			MessageIncomming() = default;
			MessageIncomming(std::shared_ptr<Client> client_, int id_, const std::string& name_, const nlohmann::json& data_);

			void setClient(const std::shared_ptr<Client>& client_);
			const std::shared_ptr<Client>& getClient() const;

			void reply(const nlohmann::json& data);
			void reply(const nlohmann::json& data, OnMessageCallback replyCallback);
		};
	}
}