#include <mainframe/ptalk/message.h>

namespace mainframe {
	namespace ptalk {
		Message::Message() {
			id = generateUniqueId();
		}

		Message::Message(const std::string& name_, const nlohmann::json& data_) : name(name_), data(data_) {
			id = generateUniqueId();
		}

		void Message::setData(const nlohmann::json& data_) {
			data = data_;
		}

		void Message::setName(const std::string& name_) {
			name = name_;
		}

		void Message::cancel() {
			cancelled = true;
		}

		const nlohmann::json& Message::getData() const {
			return data;
		}

		const std::string& Message::getName() const {
			return name;
		}

		int Message::getId() const {
			return id;
		}

		void Message::setId(int id_) {
			id = id_;
		}

		bool Message::isCancelled() const {
			return cancelled;
		}

		int Message::generateUniqueId() {
			static std::atomic<int> _id;
			return _id++;;
		}
	}
}