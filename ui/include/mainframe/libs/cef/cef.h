#pragma once

namespace mainframe {
	namespace cef {
		class Cef {
			bool inited = false;

		public:
			int init();
			void tick();

			~Cef();
		};
	}
}