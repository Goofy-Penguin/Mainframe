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
			operator char() const { return raw[0]; }
			operator unsigned char() const { return static_cast<unsigned char>(raw[0]); }

			operator short() const { return static_cast<short>(std::stoi(raw)); }
			operator unsigned short() const { return static_cast<unsigned short>(std::stoul(raw)); }

			operator int() const { return std::stoi(raw); }
			operator unsigned int() const { return static_cast<unsigned int>(std::stoul(raw)); }

			operator long() const { return std::stol(raw); }
			operator unsigned long() const { return static_cast<unsigned int>(std::stoul(raw)); }

			operator long long() const { return std::stoll(raw); }
			operator unsigned long long() const { return std::stoull(raw); }

			operator float() const { return std::stof(raw); }
			operator double() const { return std::stod(raw); }
			operator long double() const { return std::stold(raw); }

			operator std::string() const { return raw; }
			operator bool() const { return std::stoll(raw) > 0; }

			template<class T>
			T get() const {
				return *this;
			}

			Value() = default;
			~Value() = default;

			Value(const std::string& value, Type _type);
			Value(const Value& value);

			Type getType() const;
		};
	}
}