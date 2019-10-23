#include <mainframe/networking/packet.h>
#include <fstream>
#include <iostream>

namespace mainframe {
	namespace networking {
		bool Packet::seek(size_t offset) {
			if (offset > end()) return false;

			pos = offset;
			return true;
		}

		bool Packet::readToFile(const std::string& filename) {
			std::fstream  file(filename, std::ios::out | std::ios::binary | std::ios::trunc);
			if (!file.is_open()) return false;

			file.write(reinterpret_cast<char*>(buffer.data()) + pos, size() - pos);
			file.close();

			return true;
		}

		bool Packet::writeFromFile(const std::string& filename) {
			std::ifstream file(filename, std::ios::in | std::ios::binary | std::ios::ate);
			if (!file.is_open()) return false;

			auto size = file.tellg();
			std::vector<char> fileData;
			fileData.resize(static_cast<size_t>(size));

			file.seekg(0, std::ios::beg);
			file.read(fileData.data(), size);
			file.close();

			buffer.insert(buffer.begin(), fileData.begin(), fileData.end());

			return true;
		}
	}
}