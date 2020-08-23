#include <mainframe/ptalk/messageincoming.h>
#include <mainframe/ptalk/client.h>

namespace mainframe {
	namespace ptalk {
		MessageIncoming::MessageIncoming(std::shared_ptr<Client> client_, int id_, const std::string& name_, const nlohmann::json& data_) : client(client_), Message(name_, data_) {
			setId(id_);
		}

		void MessageIncoming::setClient(const std::shared_ptr<Client>& client_) {
			client = client_;
		}

		const std::shared_ptr<Client>& MessageIncoming::getClient() const {
			return client;
		}

		void MessageIncoming::reply(const nlohmann::json& data) {
			client->reply(*this, data);
		}

		void MessageIncoming::reply(const nlohmann::json& data, OnMessageCallback replyCallback) {
			client->reply(*this, data, replyCallback);
		}
	}
}