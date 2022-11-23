#include "TabBase.h"

namespace sip
{
	TabBase::TabBase(const SizeF& tabSize, const Array<String>& items)
		: tab_size_{ tabSize }
		, items_{ items }
		, active_index_{ 0 }
	{
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
