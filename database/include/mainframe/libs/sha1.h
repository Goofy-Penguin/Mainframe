#pragma once

#include <vector>
#include <string>
#include <inttypes.h>

namespace mainframe {
	namespace sha1 {
		using Basetype = uint32_t;
		using Hash = std::vector<Basetype>;

		Hash sha1(const std::string& input);
		Hash sha1(const std::vector<unsigned char>& input);

		Hash sha1(const void* pMem, Basetype iLen, Hash& my_hash);
		Hash sha1(const void* mem, int len);
	}
}