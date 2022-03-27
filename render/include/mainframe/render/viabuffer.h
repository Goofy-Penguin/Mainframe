#pragma once

namespace mainframe {
	namespace render {
		class VIABuffer {
		private:
			unsigned int vao = 0xFFFFFFFF;
			unsigned int vbo = 0xFFFFFFFF;
			unsigned int ebo = 0xFFFFFFFF;

		public:
			VIABuffer();
			~VIABuffer();

			unsigned int getVao();
			unsigned int getVbo();
			unsigned int getEbo();
		};
	}
}