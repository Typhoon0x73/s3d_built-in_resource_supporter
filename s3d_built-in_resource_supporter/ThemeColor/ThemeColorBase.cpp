#include "ThemeColorBase.h"
#include "../stdafx.h"

namespace sip
{
	ThemeColorBase::ThemeColorBase(const HashTable<ColorTag, ColorF>& colors) noexcept
		: colors_{ colors }
	{
	}

	const ColorF& ThemeColorBase::getColor(ColorTag tag) const noexcept
	{
		auto color = colors_.find(tag);
		if (color != colors_.end())
		{
			return color->second;
		}
		return invalid_color;
	}
}
