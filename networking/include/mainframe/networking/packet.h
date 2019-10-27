#pragma once

#include <string>
#include <cstring>
#include <vector>
#include <array>
#include <algorithm>

namespace mainframe {
	namespace networking {
		enum class EndianType {
			System, Big, Little
		};

		class Packet {
		private:
			std::vector<uint8_t> buffer;
			size_t pos = 0;

			template <typename T>
			struct  is_container {
				static const bool value = false;
			};

			template <typename T, typename Alloc>
			struct  is_container<std::vector<T, Alloc> > {
				static const bool value = true;
			};

		public:
			template<class T>
			T read() {
				if constexpr (is_container<T>::value) {
					auto elms = read<uint16_t>();

					T ret;
					for (uint16_t i = 0; i < elms; i++) {
						ret.push_back(read<typename T::value_type>());
					}

					return ret;
				}

				T ret;
				std::memcpy(&ret, &buffer.at(pos), sizeof(T));
				pos += sizeof(const T);

				return ret;
			}

			std::string readString() {
				auto len = read<uint16_t>();
				pos += len;
				return {buffer.begin() + pos - len, buffer.begin() + pos};
			}

			std::string readAllString() {
				auto len = size() - pos;
				pos += len;
				return {buffer.end() - len, buffer.end()};
			}

			bool readToFile(const std::string& filename);
			bool writeFromFile(const std::string& filename);

			bool seek(size_t offset);
			inline size_t begin() const { return 0; }
			inline size_t tell() const { return pos; }
			inline size_t end() const { return buffer.size(); }
			inline size_t size() const { return buffer.size(); }
			inline auto data() { return buffer.data(); }
			inline auto data() const { return buffer.data(); }
			inline bool empty() const { return buffer.empty(); }
			inline const std::vector<uint8_t>& readAll() const { return buffer; }

			template<class T>
			void write(const T& obj) {
				auto ptr = reinterpret_cast<const uint8_t*>(&obj);

				buffer.insert(buffer.begin() + pos, ptr, ptr + sizeof(const T));
				pos += sizeof(const T);
			}

			template<class T>
			void write(const std::string& obj) {
				write(obj, true);
			}

			template<class T>
			void write(std::vector<T> arr) {
				write(arr.begin(), arr.end());
			}

			template<class IterType>
			void write(IterType begin, IterType end, bool writeLength = true) {
				if (writeLength) write(static_cast<uint16_t>(std::distance(begin, end)));

				std::for_each(begin, end, [this](const auto& row) {
					write(row);
				});
			}

			void write(const std::string& obj, bool writeLength = true) {
				write(obj.begin(), obj.end(), writeLength);
			}

			template<class T>
			inline operator T() { return this->read<T>(); }

			inline operator std::string() { return read<std::string>(); }
		};
	}
}