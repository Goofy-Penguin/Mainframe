#include "mainframe/math/vector2.h"
#include <fmt/printf.h>

bool didErr = false;

void err(const std::string& msg) {
	didErr = true;
	fmt::print("{}\n", msg);
}

int main() {
	// bounds checks
	mainframe::math::Vector2 val = {0.5f, 0.5f};
	mainframe::math::Vector2 min = {};
	mainframe::math::Vector2 max = {1, 1};
	if (val < min) err("< failed");
	if (val <= min) err("<= failed");

	if (val > max) err("> failed");
	if (val >= max) err(">= failed");

	// member math operators
	val = 1;
	if ((val / 2).x != val.x / 2) err("x / failed");
	if ((val * 2).x != val.x * 2) err("x * failed");
	if ((val - 2).x != val.x - 2) err("x - failed");
	if ((val + 2).x != val.x + 2) err("x + failed");

	if ((val / 2).y != val.y / 2) err("y / failed");
	if ((val * 2).y != val.y * 2) err("y * failed");
	if ((val - 2).y != val.y - 2) err("y - failed");
	if ((val + 2).y != val.y + 2) err("y + failed");

	// self assignment operators
	val = {};
	val += 1;
	if (val.x != 1 || val.y != 1) err("+= failed");

	val = {};
	val -= 1;
	if (val.x != -1 || val.y != -1) err("-= failed");

	val = 1;
	val *= 2;
	if (val.x != 2 || val.y != 2) err("*= failed");

	val = 1;
	val /= 2;
	if (val.x != 0.5f || val.y != 0.5f) err("/= failed");

	// the end
	if (didErr) {
		return 1;
	}

	fmt::print("All tests successfull\n");
	return 0;
}