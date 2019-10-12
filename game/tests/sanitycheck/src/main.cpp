#include "game/engine.h"

int main() {
	mainframe::game::Engine e;

	while (true) e.tick();

	return 0;
}