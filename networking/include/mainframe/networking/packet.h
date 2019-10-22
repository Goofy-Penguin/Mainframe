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
			std::vector<uint8_t> _buffer;
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

				const T* ret = reinterpret_cast<const T*>(&_buffer.at(pos));
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
				auto len = read<uint16_t>();
				pos += len;
				return {_buffer.begin() + pos - len, _buffer.begin() + pos};
			}

			std::string readAllString() {
				auto len = size() - pos;
				pos += len;
				return {_buffer.end() - len, _buffer.end()};
			}

			bool readToFile(const std::string& filename);
			bool writeFromFile(const std::string& filename);

			const std::vector<uint8_t>& buffer();
			bool seek(size_t offset);
			inline size_t begin() const { return 0; }
			inline size_t tell() const { return pos; }
			inline size_t end() const { return _buffer.size(); }
			inline size_t size() const { return _buffer.size(); }
			inline auto data() { return _buffer.data(); }
			inline auto data() const { return _buffer.data(); }
			inline bool empty() const { return _buffer.empty(); }
			inline const std::vector<uint8_t>& readAll() const { return _buffer; }

			template<class T>
			void write(const T& obj) {
				auto ptr = reinterpret_cast<const uint8_t*>(&obj);

				_buffer.insert(_buffer.begin() + pos, ptr, ptr + sizeof(const T));
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