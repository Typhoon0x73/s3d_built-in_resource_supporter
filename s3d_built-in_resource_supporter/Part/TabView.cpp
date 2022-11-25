#include "TabView.h"
#include "../stdafx.h"
#include "../Tab/SimpleTab.h"
#include "../ThemeColor/ThemeColorManager.h"

namespace sip
{
	TabView::TabView(const Array<String>& items, const Vec2& pos) noexcept
		: draw_pos_{ pos }
		, tab_{ new SimpleTab(SimpleGUI::GetFont(), pos, Size{ 100, 30 }, items) }
	{
	}

	TabView::~TabView() noexcept
	{
	}

	void TabView::update() noexcept
	{
		if (tab_ == nullptr)
		{
			return;
		}
		for (int32 i = 0; i < tab_->getTabCount(); ++i)
		{
			if (!tab_->getTabRect(i).movedBy(draw_pos_).leftClicked())
			{
				continue;
			}
			tab_->setActiveTabIndex(i);
			return;
		}
	}

	void TabView::draw() const noexcept
	{
		if (tab_ == nullptr)
		{
			return;
		}
		tab_->draw();
	}

	void TabView::setPos(const Vec2& pos) noexcept
	{
		draw_pos_ = pos;
	}

	size_t TabView::getSelectTabNo() const noexcept
	{
		if (tab_ == nullptr)
		{
			return 0;
		}
		return tab_->getActiveTabIndex();
	}

	const Vec2& TabView::getPos() const noexcept
	{
		return draw_pos_;
	}

	const RectF TabView::getTabRect(size_t index) const noexcept
	{
		if (tab_ == nullptr)
		{
			return RectF{};
		}
		return tab_->getTabRect(index);
	}
}
