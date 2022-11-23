#pragma once
#ifndef SIP_THEME_COLOR_BASE_H_
#define SIP_THEME_COLOR_BASE_H_

#include <Siv3D.hpp>
#include "ThemeDefine.h"

namespace sip
{
	/// @brief 
	constexpr ColorF invalid_color{ ColorF::Zero() };

	/// @brief 
	class ThemeColorBase
	{
	public:

		/// @brief 
		/// @param colors 
		explicit ThemeColorBase(const HashTable<ColorTag, ColorF>& colors) noexcept;

		/// @brief 
		virtual ~ThemeColorBase() noexcept = default;

		/// @brief 
		/// @param tag 
		/// @return 
		const ColorF& getColor(ColorTag tag) const noexcept;

	private:

		/// @brief 
		const HashTable<ColorTag, ColorF> colors_;
	};

	using ThemeColorPtr = std::unique_ptr<ThemeColorBase>;
}

#endif // !SIP_THEME_COLOR_BASE_H_
