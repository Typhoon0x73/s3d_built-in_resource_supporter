#include "TabBase.h"

namespace sip
{
	TabBase::TabBase(const Font& font, const Vec2& pos, const SizeF& tabSize, const Array<String>& items)
		: tab_size_{ tabSize }
		, items_{ items }
		, active_index_{ 0 }
		, pos_{ pos }
		, font_{ font }
	{
	}

	void TabBase::update()
	{
	}

	const Vec2& TabBase::getPos() const noexcept
	{
		return pos_;
	}

	size_t TabBase::getTabCount() const noexcept
	{
		return items_.size();
	}

	size_t TabBase::getActiveTabIndex() const noexcept
	{
		return active_index_;
	}

	void TabBase::setActiveTabIndex(size_t index) noexcept
	{
		assert(index < items_.size());
		active_index_ = index;
	}

	void TabBase::setPos(const Vec2& pos) noexcept
	{
		pos_ = pos;
	}

	void TabBase::setFont(const Font& font) noexcept
	{
		font_ = font;
	}

	void TabBase::advance(int32 offset, bool wrapAround)
	{

		assert(InRange(offset, -1, 1));

		if (offset == -1)
		{
			if (active_index_ == 0)
			{
				if (wrapAround)
				{
					active_index_ = (items_.size() - 1);
				}
			}
			else
			{
				--active_index_;
			}
		}
		else if (offset == 1)
		{
			if (active_index_ == (items_.size() - 1))
			{
				if (wrapAround)
				{
					active_index_ = 0;
				}
			}
			else
			{
				++active_index_;
			}
		}
	}
}
