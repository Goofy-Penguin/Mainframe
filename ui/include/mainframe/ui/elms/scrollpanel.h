#pragma once

#include <mainframe/ui/element.h>
#include <mainframe/ui/elms/scrollbar.h>
#include <mainframe/ui/elms/scrollpanelback.h>

namespace mainframe {
	namespace ui {
		class ScrollPanel : public Element {
			math::Vector2i offsetSize;
			math::Vector2i offset;
			std::shared_ptr<ScrollBar<int>> barVertical;
			std::shared_ptr<ScrollBar<int>> barHorizontal;
			std::shared_ptr<ScrollPanelBack> backPanel;
			bool showButtons = false;
			int barSize = 20;

			void updateOffsetSize();

		public:
			virtual void draw(render::Stencil& stencil) override;

			virtual void setSize(const math::Vector2i& size_) override;

			void setButtons(bool buttons);
			void setBars(bool v, bool h = false);
			void updateBars();

			const std::shared_ptr<ScrollPanelBack>& getPanel() const;
		};
	}
}