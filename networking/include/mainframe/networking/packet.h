#pragma once

#include <string>
#include <vector>
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
				using subtype = T;
			};

		public:
			template<class T>
			T read() {
				if constexpr (is_container<T>::value) {
					auto elms = read<size_t>();

					T ret;
					for (size_t i = 0; i < elms; i++) {
						ret.push_back(read<is_container<T>::subtype>());
					}

					return ret;
				}

				const T* ret = reinterpret_cast<const T*>(&buffer.at(pos));
				pos += sizeof(const T);

				return *ret;
			}

			template<class T, size_t SIZE>
			std::array<T, SIZE> read() {
				auto ret = {};
				for (size_t i = 0; i < SIZE; i++) {
					ret[i] = read();
				}

				return ret;
			}

			template<>
			std::string read() {
				auto len = read<size_t>();
				return {buffer.begin() + pos, buffer.begin() + pos + len};
			}

			std::string readAllString() {
				return {buffer.begin() + pos, buffer.end()};
			}

			bool seek(size_t offset);
			inline size_t begin() { return 0; }
			inline size_t tell() { return pos; }
			inline size_t end() { return buffer.size(); }
			inline const std::vector<uint8_t>& readAll() { return buffer; }

			template<class T>
			void write(const T& obj) {
				auto ptr = reinterpret_cast<const uint8_t*>(&obj);

				buffer.insert(buffer.begin() + pos, ptr, ptr + sizeof(const T));
				pos += sizeof(const T);
			}

			template<>
			void write(const std::string& obj) {
				write(obj, true);
			}

			template<class T>
			void write(std::vector<T> arr) {
				write(arr.begin(), arr.end());
			}

			template<class T>
			void write(T begin, T end, bool writeLength = true) {
				if (writeLength) write(std::distance(begin, end));

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