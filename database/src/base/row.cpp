#include <mainframe/database/base/row.h>
#include <stdexcept>
#include <algorithm>

namespace mainframe {
	namespace database {
		Row::Row(const std::shared_ptr<std::vector<std::string>>& _columns) : columns(_columns) {
		}

		bool Row::has(const std::string& key) const {
			return find(key) != end();
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

		std::vector<std::string>::iterator Row::find(const std::string& key) const {
			return std::find(columns->begin(), columns->end(), key);
		}

		std::vector<std::string>::iterator Row::begin() const {
			return columns->begin();
		}

		std::vector<std::string>::iterator Row::end() const {
			return columns->end();
		}

		const std::shared_ptr<std::vector<std::string>>& Row::getColumns() const {
			return columns;
		}

		void Row::setColumns(const std::shared_ptr<std::vector<std::string>>& columns) {
			this->columns = columns;
		}
	}
}