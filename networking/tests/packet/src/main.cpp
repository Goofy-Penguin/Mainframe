#include "mainframe/networking/packet.h"
#include <typeinfo>
#include <fmt/printf.h>

using namespace mainframe::networking;

bool failed = false;

template<class T>
void testType(const T& obj, LengthType ltype = LengthType::None) {
	Packet p;
	p.setlengthFormat(ltype);

	p.write(obj);
	p.seek(p.begin());

	T val = p.read<T>();
	if (obj != val) {
		fmt::print("test failed for type {}\n", typeid(T).name());
		failed = true;
		return;
	}

	fmt::print("test success for type {}\n", typeid(T).name());
}

void testStringAll(const std::string& str) {
	Packet p;

	p.write(str, false);
	p.seek(p.begin());

	auto val = p.readAllString();
	if (val != str) {
		fmt::print("test failed for testStringAll\n");
		failed = true;
		return;
	}

	fmt::print("test success for testStringAll\n");
}

void testSeek() {
	Packet p;

	p.write<int>(1);
	p.seek(p.begin());
	p.write<int>(2);
	p.seek(p.end());
	p.write<int>(3);

	bool fail = false;
	p.seek(p.begin());
	if (p.read<int>() != 2) fail = true;
	if (p.read<int>() != 1) fail = true;
	if (p.read<int>() != 3) fail = true;

	if (!fail) {
		fmt::print("test success for seek\n");
		return;
	}

	failed = true;
	fmt::print("test failed for seek\n");
}

int main() {
	testSeek();

	testType(true);
	testType(static_cast<float>(1));
	testType(static_cast<double>(1));
	testType(static_cast<uint8_t>(1));
	testType(static_cast<uint16_t>(1));
	testType(static_cast<uint32_t>(1));
	testType(static_cast<uint64_t>(1));

	testStringAll("beep boop");

	// test all length types for all objects
	int lengthTypes = static_cast<int>(LengthType::UInt64) - static_cast<int>(LengthType::UInt8);
	for (int i = 0; i <= lengthTypes; i++) {
		LengthType ltype = static_cast<LengthType>(static_cast<int>(LengthType::UInt8) + i);

		testType(std::string("hello world"), ltype);
		testType(std::vector<int>{1, 2, 3}, ltype);
		testType(std::vector<std::vector<int>>{ {1, 2, 3}}, ltype);
		testType(std::array<int, 3>{ {1, 2, 3}}, ltype);
		testType(std::map<std::string, int>{ {"a", 1}, {"b", 2}, {"c", 3}}, ltype);
	}

	return failed ? -1 : 0;
}
