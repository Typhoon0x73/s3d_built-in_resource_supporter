#include "SimpleTab.h"

namespace sip
{
	SimpleTab::SimpleTab(const SizeF& tabSize, const Array<String>& items)
		: TabBase{ tabSize, items }
	{
	}

	void SimpleTab::draw(const Vec2& pos, const Font& font, const ColorF& color, const ColorF&) const
	{
		constexpr double Thickness = 10.0;
		const double radius = (tab_size_.y * 0.25);

		for (size_t i = 0; i < items_.size(); ++i)
		{
			RectF tab = getTabRect(i).movedBy(pos);

			ColorF base_color = color;
			Vec2 topleft{ -2, -2 };
			Vec2 bottomright{ 2, 2 };
			
			if (i == active_index_)
			{
				std::swap(bottomright, topleft);
			}
			if (tab.mouseOver())
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
			const RectF tab = getTabRect(i).movedBy(pos);

			font(items_[i])
				.drawAt(
					tab.h * 0.65, tab.center(), Palette::Dimgray
				);
		}
	}

	const RectF SimpleTab::getTabRect(size_t index) const noexcept
	{
		constexpr double Thickness = 10.0;
		const RectF tab{ (index * (tab_size_.x + Thickness)), 0.0, tab_size_ };
		return tab;
	}
}
