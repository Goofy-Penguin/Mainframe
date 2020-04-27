#pragma once

#include <vector>
#include <string>

namespace mainframe {
	namespace utils {
		namespace crc {
			uint32_t crc32(const uint8_t* data, size_t length);
			uint32_t crc32(const std::vector<uint8_t>& data);

			template<size_t size>
			uint32_t crc32(const std::array<uint8_t, size>& data) {
				return crc32(data.data(), data.size());
			}
		};
	}
}