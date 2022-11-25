#pragma once
#ifndef SIP_ROUND_TAB_H_
#define SIP_ROUND_TAB_H_

#include "TabBase.h"

namespace sip
{
	/// @brief 
	class SimpleTab : public TabBase
	{
	public:

		/// @brief 
		/// @param font 
		/// @param pos 
		/// @param tabSize 
		/// @param items 
		explicit SimpleTab(const Font& font, const Vec2& pos, const SizeF& tabSize, const Array<String>& items);

		/// @brief 
		virtual void update() override;

		/// @brief すべてのタブを描画します。
		virtual void draw() const override;

		/// @brief 
		/// @param index 
		/// @return 
		virtual const RectF getTabRect(size_t index) const noexcept override;
	};
}

#endif // !SIP_ROUND_TAB_H_
