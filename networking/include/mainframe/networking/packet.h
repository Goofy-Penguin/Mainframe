#pragma once

#include <string>
#include <cstring>
#include <vector>
#include <array>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>

namespace mainframe {
	namespace networking {
		class Packet;
		template<typename T>
		concept isNetworkable = requires(T t, Packet p) {
			{ t.networkRead(p) };
			{ t.networkWrite(p) };
		};

		enum class EndianType {
			System, Big, Little
		};

		enum class LengthType {
			None, UInt8, UInt16, UInt32, UInt64
		};

		class Packet {
		private:
			std::vector<uint8_t> buffer;
			size_t pos = 0;

			LengthType lengthFormat = LengthType::None;
			static LengthType lengthFormatGlobal;

		public:
			template<class T>
			T read() {
				T ret;
				read(ret);
				return ret;
			}

			template<class T>
			void read(T& ret) {
    			if constexpr (isNetworkable<T>) {
					ret.networkRead(*this);
				} else {
					static_assert(std::is_trivially_copyable_v<T>, "Fallback option for not a (vector, map, string, and does not supply a networkRead) is 'just memcpy it lel', but that needs T to be trivially copyable, or we're potentially memcpying classes that have strings in them or somesuch.");
					if (pos + sizeof(T) > buffer.size()) {
						throw std::runtime_error("reading past buffer");
					}

					std::memcpy(&ret, &buffer.at(pos), sizeof(T));
					pos += sizeof(const T);
				}
			}

			template<class T>
			void read(std::vector<T>& ret) {
				size_t elms = readLength<size_t>();
				if(elms <= 0) return;

				while (elms-- > 0) {
					ret.push_back(read<T>());
				}
			}

			template<class T, size_t size>
			void read(std::array<T, size>& ret) {
				for (size_t i = 0; i < size; i++) {
					read<T>(ret[i]);
				}
			}

			template<class A, class B>
			void read(std::pair<A, B>& ret) {
				ret.first = read<A>();
				ret.second = read<B>();
			}

			template<class A, class B>
			void read(std::map<A, B>& ret) {
				size_t elms = readLength<size_t>();
				if(elms <= 0) return;

				while (elms-- > 0) {
					ret.emplace(read<std::pair<A, B>>());
				}
			}

			template<class A, class B>
			void read(std::unordered_map<A, B>& ret) {
				size_t elms = readLength<size_t>();
				if(elms <= 0) return;

				while (elms-- > 0) {
					ret.insert(read<std::pair<A, B>>());
				}
			}

			void read(std::string& ret) {
				auto len = readLength<size_t>();
				auto start = buffer.begin() + pos;

				ret.assign(start, start + len);
				pos += len;
			}

			std::string readAllString() {
				auto len = size() - pos;
				pos += len;
				return {buffer.end() - len, buffer.end()};
			}

			bool readToFile(const std::string& filename);
			bool writeFromFile(const std::string& filename);

			bool seek(size_t offset);
			bool seek(std::vector<uint8_t>::iterator offset);
			inline size_t tell() const { return pos; }
			inline size_t size() const { return buffer.size(); }
			inline auto data() { return buffer.data(); }
			inline auto data() const { return buffer.data(); }
			inline auto& getBuffer() { return buffer; }
			inline auto& getBuffer() const { return buffer; }
			inline void setBuffer(std::vector<uint8_t> b) { buffer = b; }

			inline auto begin() noexcept { return buffer.begin(); }
			inline auto end() noexcept { return buffer.end(); }
			inline auto cbegin() const noexcept { return buffer.cbegin(); }
			inline auto cend() const noexcept { return buffer.cend(); }

			inline void resize(size_t size) { buffer.resize(size); }

			inline bool empty() const { return buffer.empty(); }
			inline const std::vector<uint8_t>& readAll() const { return buffer; }
			void clear();

			template<class T>
			void write(const T& obj) {
				if constexpr (isNetworkable<T>) {
					obj.networkWrite(*this);
				} else {
					static_assert(std::is_trivially_copyable_v<T>, "Fallback option for not a (vector, map, string, and does not supply a networkWrite) is 'just reinterpret_cast it lel', but that needs T to be trivially copyable, or we're potentially reinterpreting classes that have strings in them or somesuch.");
					auto ptr = reinterpret_cast<const uint8_t*>(&obj);

					buffer.insert(buffer.begin() + pos, ptr, ptr + sizeof(const T));
					pos += sizeof(const T);
				}
			}

			template<class T>
			void write(const std::string& obj, bool shouldWriteLength = true) {
				write(obj.begin(), obj.end(), shouldWriteLength);
			}

			template<class T>
			void write(const std::vector<T>& obj, bool shouldWriteLength = true) {
				write(obj.begin(), obj.end(), shouldWriteLength);
			}

			template<class A, class B>
			void write(const std::pair<A, B>& obj) {
				write(obj.first);
				write(obj.second);
			}

			template<class A, class B>
			void write(const std::map<A, B>& obj, bool shouldWriteLength = true) {
				write(obj.begin(), obj.end(), shouldWriteLength);
			}

			template<class A, class B>
			void write(const std::unordered_map<A, B>& obj, bool shouldWriteLength = true) {
				write(obj.begin(), obj.end(), shouldWriteLength);
			}

			template<class T, size_t size>
			void write(const std::array<T, size>& obj, bool shouldWriteLength = false) {
				write(obj.begin(), obj.end(), shouldWriteLength);
			}

			template<class IterType>
			void write(IterType begin, IterType end, bool shouldWriteLength = true) {
				if (shouldWriteLength) writeLength(std::distance(begin, end));

				std::for_each(begin, end, [this](const auto& row) {
					write(row);
				});
			}

			void write(const std::string& obj, bool shouldWriteLength = true) {
				write(obj.begin(), obj.end(), shouldWriteLength);
			}


			template<class T>
			void writeLength(T size) {
				auto format = lengthFormat;
				if (format == LengthType::None)
					format = lengthFormatGlobal;

				switch (format) {
					case LengthType::None: throw std::runtime_error("lengthFormat cannot be None");
					case LengthType::UInt8: write(static_cast<uint8_t>(size)); break;
					case LengthType::UInt16: write(static_cast<uint16_t>(size)); break;
					case LengthType::UInt32: write(static_cast<uint32_t>(size)); break;
					case LengthType::UInt64: write(static_cast<uint64_t>(size)); break;
					default: throw std::runtime_error("unknown lengthFormat");
				}
			}

			template<class T>
			T readLength() {
				auto format = lengthFormat;
				if (format == LengthType::None)
					format = lengthFormatGlobal;

				switch (format) {
					case LengthType::None: throw std::runtime_error("lengthFormat cannot be None");
					case LengthType::UInt8: return static_cast<T>(read<uint8_t>());
					case LengthType::UInt16: return static_cast<T>(read<uint16_t>());
					case LengthType::UInt32: return static_cast<T>(read<uint32_t>());
					case LengthType::UInt64: return static_cast<T>(read<uint64_t>());
					default: throw std::runtime_error("unknown lengthFormat");
				}
			}

			// LengthType::None refers to Global, defaults to UInt16
			void setlengthFormat(LengthType format) {
				lengthFormat = format;
			}

			static void setlengthFormatGlobal(LengthType format) {
				if (format == LengthType::None) throw std::runtime_error("cannot set LengthType::None as global length, used for global to local override");
				lengthFormatGlobal = format;
			}

			LengthType getLengthFormat() const {
				return lengthFormat == LengthType::None ? lengthFormatGlobal : lengthFormat;
			}

			template<class T>
			inline operator T() { return this->read<T>(); }
		};
	}
}
