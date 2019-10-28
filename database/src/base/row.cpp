#include <mainframe/database/base/row.h>
#include <stdexcept>

namespace mainframe {
	namespace database {
		Row::Row(const std::shared_ptr<std::vector<std::string>>& _columns) : columns(_columns) {
		}

		const Value& Row::get(size_t index) const {
			return values[index];
		}

		const Value& Row::get(const std::string& key) const {
			auto& colms = *columns;
			for (size_t i = 0; i < colms.size(); i++) {
				if (key == colms[i]) {
					return values[i];
				}
			}

			throw std::runtime_error("value not found");
		}

		const Value& Row::operator[](const std::string& key) const {
			return get(key);
		}

		const Value& Row::operator[](size_t index) const {
			return get(index);
		}
	}
}