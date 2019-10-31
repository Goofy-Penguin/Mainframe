#pragma once

namespace mainframe {
	namespace render {
		class VIABuffer {
		private:
			unsigned int vao = -1;
			unsigned int vbo = -1;
			unsigned int ebo = -1;

		public:
			VIABuffer();
			~VIABuffer();

			unsigned int getVao();
			unsigned int getVbo();
			unsigned int getEbo();
		};
	}
}