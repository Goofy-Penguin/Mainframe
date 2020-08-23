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

		uint32_t Message::getId() const {
			return id;
		}

		void Message::setId(uint32_t id_) {
			id = id_;
		}

		bool Message::isCancelled() const {
			return cancelled;
		}

		uint32_t Message::generateUniqueId() {
			static std::atomic<uint32_t> _id;
			return _id++;;
		}
	}
}