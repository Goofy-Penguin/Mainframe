#include <mainframe/ui/elms/scrollpanel.h>

namespace mainframe {
	namespace ui {
		using namespace mainframe::render;

		void ScrollPanel::draw(render::Stencil& stencil) {
			stencil.drawBoxOutlined({0, 0}, getSize().cast<float>(), static_cast<float>(borderSize), colorBorder);
			stencil.drawBox(static_cast<float>(borderSize), getSize().cast<float>() - static_cast<float>(borderSize) * 2, colorBackground);
		}

		const std::shared_ptr<ScrollPanelBack>& ScrollPanel::getPanel() const {
			return backPanel;
		}

		void ScrollPanel::setButtons(bool buttons) {
			showButtons = buttons;
		}

		void ScrollPanel::setBackgroundColor(const render::Color& color) {
			colorBackground = color;
		}

		void ScrollPanel::setBackgroundBorderColor(const render::Color& color) {
			colorBorder = color;
		}

		void ScrollPanel::setBorderSize(int size) {
			borderSize = size;
		}

		void ScrollPanel::setBars(bool v, bool h) {
			if (!backPanel) {
				backPanel = createChild<ScrollPanelBack>();
			}

			if (v) {
				barVertical = createChild<ScrollBar<int>>();
				barVertical->onValueChanged += [this]() {
					offset.y = -barVertical->getValue();
					backPanel->setOffset(offset);
				};
			} else if (barVertical) {
				barVertical->remove();
				barVertical = nullptr;
			}

			if (h) {
				barHorizontal = createChild<ScrollBar<int>>();
				barHorizontal->onValueChanged += [this]() {
					offset.x = -barHorizontal->getValue();
					backPanel->setOffset(offset);
				};
			} else if (barHorizontal) {
				barHorizontal->remove();
				barHorizontal = nullptr;
			}
		}

		void ScrollPanel::updateOffsetSize() {
			offsetSize = {};
			for (auto& child : backPanel->getChildren()) {
				auto aabb = child->getAABB();

				if (aabb.pos.x + aabb.size.x > offsetSize.x) offsetSize.x = aabb.pos.x + aabb.size.x;
				if (aabb.pos.y + aabb.size.y > offsetSize.y) offsetSize.y = aabb.pos.y + aabb.size.y;
			}
		}

		void ScrollPanel::updateBars() {
			updateOffsetSize();

			auto size = getSize();
			if (barVertical) size.x -= barSize;
			if (barHorizontal) size.y -= barSize;

			if (barVertical) {
				int max = 0;
				if (offsetSize.y > size.y) max = offsetSize.y - size.y;

				barVertical->setMax(max);
			}

			if (barHorizontal) {
				int max = 0;
				if (offsetSize.x > size.x) max = offsetSize.x - size.x;

				barHorizontal->setMax(max);
			}
		}

		void ScrollPanel::setSize(const math::Vector2i& size_) {
			Element::setSize(size_);

			if (barVertical) {
				barVertical->setPos({size_.x - barSize, 0});
				barVertical->setSize({barSize, size_.y - (barHorizontal ? barSize : 0)});
				barVertical->setScrollStyle(ScrollStyle::vertical, showButtons);
			}

			if (barHorizontal) {
				barHorizontal->setPos({0, size_.y - barSize});
				barHorizontal->setSize({size_.x - (barVertical ? barSize : 0), barSize});
				barHorizontal->setScrollStyle(ScrollStyle::horizontal, showButtons);
			}

			if (backPanel) {
				auto size = size_;
				if (barVertical) size.x -= barSize;
				if (barHorizontal) size.y -= barSize;

				backPanel->setSize(size);
			}
		}
	}
}