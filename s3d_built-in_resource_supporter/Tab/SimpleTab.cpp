#include "SimpleTab.h"

namespace sip
{
	SimpleTab::SimpleTab(const Font& font, const Vec2& pos, const SizeF& tabSize, const Array<String>& items)
		: TabBase{ font, pos, tabSize, items }
		, mouse_over_no_{ -1 }
	{
	}

	void SimpleTab::update()
	{
		mouse_over_no_ = -1;
		for (size_t i = 0; i < getTabCount(); ++i)
		{
			const auto& tab = getTabRect(i);
			if (tab.mouseOver())
			{
				mouse_over_no_ = static_cast<int32>(i);
			}
			if (!tab.leftClicked())
			{
				continue;
			}
			setActiveTabIndex(i);
			return;
		}
	}

	void SimpleTab::draw() const
	{
		const double radius = (tab_size_.y * 0.25);

		for (size_t i = 0; i < items_.size(); ++i)
		{
			RectF tab = getTabRect(i);

			ColorF base_color = Palette::Lightgray;
			Vec2 topleft{ -2, -2 };
			Vec2 bottomright{ 2, 2 };
			
			if (i == active_index_)
			{
				std::swap(bottomright, topleft);
			}
			if (i == mouse_over_no_)
			{
				base_color = Palette::Gainsboro;
			}

			tab.rounded(radius)
				.drawShadow(topleft, 5.0, 0.0, Palette::Whitesmoke)
				.drawShadow(bottomright, 5.0, 0.0, Palette::Darkgray)
				.draw(base_color);
		}

		for (size_t i = 0; i < items_.size(); ++i)
		{
			const RectF tab = getTabRect(i);

			font_(items_[i])
				.drawAt(
					tab.h * 0.65, tab.center(), Palette::Dimgray
				);
		}
	}

	const RectF SimpleTab::getTabRect(size_t index) const noexcept
	{
		constexpr double Thickness = 10.0;
		const RectF tab{ (index * (tab_size_.x + Thickness)) + pos_.x, pos_.y, tab_size_ };
		return tab;
	}
}
