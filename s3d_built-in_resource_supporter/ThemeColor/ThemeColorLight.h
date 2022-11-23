#pragma once
#ifndef SIP_THEME_COLOR_LIGHT_H_
#define SIP_THEME_COLOR_LIGHT_H_

#include <Siv3D.hpp>
#include "ThemeDefine.h"

namespace sip
{
	static const HashTable<ColorTag, ColorF> theme_color_light =
	{
		{ ColorTag::MainBackground, Palette::Lightgray  },
		{ ColorTag::DialogCover   , ColorF{ 0.95, 0.45 }},
	};
}

#endif // !SIP_THEME_COLOR_LIGHT_H_
