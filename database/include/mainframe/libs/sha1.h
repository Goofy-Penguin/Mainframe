#pragma once

#include <vector>
#include <inttypes.h>

namespace mainframe {
	namespace sha1 {
		using Basetype = uint32_t;
		using Hash = std::vector<Basetype>;

		template<typename T>
		Hash sha1(const T& input) {
			Hash h(5);
			h[0] = 0x67452301, h[1] = 0xEFCDAB89,
				h[2] = 0x98BADCFE, h[3] = 0x10325476,
				h[4] = 0xC3D2E1F0;
			Hash raw = sha1(input.data(), input.size() * sizeof(*input.begin()), h);
			return raw;
		}

		Hash sha1(const void* pMem, Basetype iLen, Hash& my_hash);
		Hash sha1(const void* mem, int len);
	}
}