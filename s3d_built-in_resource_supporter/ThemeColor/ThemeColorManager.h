#pragma once
#ifndef SIP_THEME_COLOR_MANAGER_H_
#define SIP_THEME_COLOR_MANAGER_H_

#include "ThemeColorBase.h"
#include "ThemeDefine.h"

namespace sip
{
	/// @brief 
	class ThemeColorManager
	{
	public:

		/// @brief  
		explicit ThemeColorManager() noexcept;

		/// @brief 
		~ThemeColorManager() noexcept;

		/// @brief 
		/// @param theme 
		void SetTheme(ThemeColor theme) noexcept;

		/// @brief 
		/// @return 
		const ColorF& getMainBackground() const noexcept;

		/// @brief 
		/// @return 
		const ColorF& getDialogCover() const noexcept;

	private:

		/// @brief 
		ThemeColorPtr theme_color_;
	};
}

#endif // !SIP_THEME_COLOR_MANAGER_H_
