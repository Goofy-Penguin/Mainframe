#pragma once

#include <string>
#include <vector>
#include <variant>

namespace mainframe {
	namespace database {
		class Value {
		public:
			enum class Type : int {
				None, String, Int, Long, Float, Double, Bool
			};

		private:
			std::string raw;
			Type type = Type::None;

		public:
			operator char() const { return isNull() ? 0 : raw[0]; }
			operator unsigned char() const { return isNull() ? 0 : static_cast<unsigned char>(raw[0]); }

			operator short() const { return isNull() ? 0 : static_cast<short>(std::stoi(raw)); }
			operator unsigned short() const { return isNull() ? 0 : static_cast<unsigned short>(std::stoul(raw)); }

			operator int() const { return isNull() ? 0 : std::stoi(raw); }
			operator unsigned int() const { return isNull() ? 0 : static_cast<unsigned int>(std::stoul(raw)); }

			operator long() const { return isNull() ? 0 : std::stol(raw); }
			operator unsigned long() const { return isNull() ? 0 : static_cast<unsigned long>(std::stoul(raw)); }

			operator long long() const { return isNull() ? 0 : std::stoll(raw); }
			operator unsigned long long() const { return isNull() ? 0 : std::stoull(raw); }

			operator float() const { return isNull() ? 0 : std::stof(raw); }
			operator double() const { return isNull() ? 0 : std::stod(raw); }
			operator long double() const { return isNull() ? 0 : std::stold(raw); }

			operator std::string() const { return raw; }
			operator bool() const { return isNull() ? false : std::stoll(raw) > 0; }

			template<class T>
			T get() const {
				return *this;
			}

			Value() = default;
			~Value() = default;

			Value(const std::string& value, Type _type);
			Value(const Value& value);

			Type getType() const;
			bool isNull() const;
		};
	}
}
