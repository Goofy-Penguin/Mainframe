#include <mainframe/database/base/value.h>

namespace mainframe {
	namespace database {
		Value::Value(const std::string& value, Type _type) : raw(value), type(_type) {}
		Value::Value(const Value& value) : raw(value.raw), type(value.type) {}
	}
}