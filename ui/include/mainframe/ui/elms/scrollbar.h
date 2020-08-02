#pragma once

#include <mainframe/ui/element.h>
#include <mainframe/ui/elms/button.h>

namespace mainframe {
	namespace ui {
		enum class ScrollStyle {
			vertical,
			horizontal
		};

		template<class T>
		class ScrollBar : public Element {
			T value = 0;
			T valueMin = 0;
			T valueMax = 0;
			T valueStep = 0;
			int btnSize = 10;
			int sliderSize = 40;
			bool grabbing = false;
			bool showButtons = false;
			ScrollStyle btnStyle = ScrollStyle::horizontal;

			std::shared_ptr<Button> btnAdd;
			std::shared_ptr<Button> btnSubstract;
			std::shared_ptr<Button> btnSlider;

			void createElms() {
				clearChildren();

				btnSlider = createChild<Button>();
				if (!showButtons) {
					addHooks();
					return;
				}

				btnAdd = createChild<Button>();
				btnSubstract = createChild<Button>();
				addHooks();
			}

			void addHooks() {
				btnSlider->onMouseDown += [this](const math::Vector2i& mousePos, unsigned int button, ModifierKey mods) {
					grabbing = true;
				};

				btnSlider->onMouseUp += [this](const math::Vector2i& mousePos, unsigned int button, ModifierKey mods) {
					grabbing = false;
				};

				btnSlider->onMouseMove += [this](const math::Vector2i& mousePos) {
					if (!grabbing) return;

					auto& size = getSize();
					T val = {};
					switch (btnStyle) {
						case ScrollStyle::horizontal: val = mousePos.x / size.x * (valueMax - valueMin); break;
						case ScrollStyle::vertical: val = mousePos.y / size.y * (valueMax - valueMin); break;
					}

					setValue(val);
				};

				if (btnAdd) {
					btnAdd->onClick += [this]() {
						setValue(value + (valueStep == 0 ? getDiff() / 10 : valueStep));
					};
				}

				if (btnSubstract) {
					btnSubstract->onClick += [this]() {
						setValue(value - (valueStep == 0 ? getDiff() / 10 : valueStep));
					};
				}
			}

			int getButtonSizeStreched() {
				auto& size = getSize();

				switch (btnStyle) {
					case ScrollStyle::horizontal: return btnSize < size.y ? size.y : btnSize; break;
					case ScrollStyle::vertical: return btnSize < size.x ? size.x : btnSize; break;
				}

				return btnSize;
			}

		public:
			mainframe::utils::Event<> onValueChanged;

			T getMin() { return valueMin; }
			T getMax() { return valueMax; }
			T getValue() { return value; }
			T getStep() { return valueStep; }
			T getDiff() { return std::abs(valueMin - valueMax); }

			void setMin(T val) { valueMin = val; setValue(value); }
			void setMax(T val) { valueMax = val; setValue(value); }
			void setStep(T val) { valueStep = val; }
			void setValue(T val) {
				if (val < valueMin) val = valueMin;
				if (val > valueMax) val = valueMax;

				if (val != value) {
					value = val;
					onValueChanged();
				}

				if (valueMin == 0 && valueMax == 0) return;

				auto& size = getSize();
				int valuePercentage = value / (valueMax - valueMin);
				int btnoffset = showButtons ? getButtonSizeStreched() : 0;
				int offset = 0;
				switch (btnStyle) {
					case ScrollStyle::horizontal: offset = valuePercentage * (size.x - btnoffset * 2); break;
					case ScrollStyle::vertical: offset = valuePercentage * (size.y - btnoffset * 2); break;
				}

				mainframe::math::Vector2i pos;
				switch (btnStyle) {
					case ScrollStyle::horizontal: pos = {offset + btnoffset, 0}; break;
					case ScrollStyle::vertical: pos = {0, offset + btnoffset}; break;
				}

				if (btnSlider) btnSlider->setPos(pos);
			}

			void setScrollStyle(ScrollStyle style, bool buttons) {
				btnStyle = style;
				showButtons = buttons;

				createElms();

				// prepare slider with hooks

				auto& size = getSize();
				int btnSizeStreched = getButtonSizeStreched();
				int btnoffset = showButtons ? btnSizeStreched : 0;
				switch (btnStyle) {
					case ScrollStyle::horizontal:
						btnSlider->setPos({btnSizeStreched, 0});
						btnSlider->setSize({sliderSize, btnSizeStreched});
						break;
					case ScrollStyle::vertical:
						btnSlider->setPos({0, btnSizeStreched});
						btnSlider->setSize({btnSizeStreched, sliderSize});
						break;
				}

				// update slider
				setValue(value);

				// go trough buttons if wanted
				if (!showButtons) return;

				//btnAdd->setText("+");
				//btnSubstract->setText("-");
				//btnSlider->setText("[]");

				// set location of buttons
				switch (style) {
					case ScrollStyle::horizontal:
					{
						btnAdd->setPos({size.x - btnSizeStreched, 0});
						btnAdd->setSize({btnSizeStreched, btnSizeStreched});

						btnSubstract->setPos({0, 0});
						btnSubstract->setSize({btnSizeStreched, btnSizeStreched});

						break;
					}

					case ScrollStyle::vertical:
					{
						btnSubstract->setPos({0, 0});
						btnSubstract->setSize({btnSizeStreched, btnSizeStreched});

						btnAdd->setPos({0, size.y - btnSizeStreched});
						btnAdd->setSize({btnSizeStreched, btnSizeStreched});

						break;
					}
				}
			}

			virtual void draw(render::Stencil& stencil) override {
				stencil.drawBox({0, 0}, getSize(), render::Color(255, 255, 255, 200));
				stencil.drawBoxOutlined({0, 0}, getSize(), 1, Colors::Black);
			}
		};
	}
}