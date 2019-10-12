#include "mainframe/networking/packet.h"
#include <typeinfo>
#include <fmt/printf.h>

using namespace mainframe::networking;

bool failed = false;

template<class T>
void testType(const T& obj) {
	Packet p;

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
	testType(true);
	testType(static_cast<float>(1));
	testType(static_cast<double>(1));
	testType(static_cast<uint8_t>(1));
	testType(static_cast<uint16_t>(1));
	testType(static_cast<uint32_t>(1));
	testType(static_cast<uint64_t>(1));
	testType(std::string("hello world"));
	testType(std::vector<int>{1, 2, 3});
	testType(std::vector<std::vector<int>>{ {1, 2, 3}});

	testStringAll("beep boop");

	testSeek();

	return failed ? -1 : 0;
}