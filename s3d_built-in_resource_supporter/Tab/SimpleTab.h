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
		/// @param tabSize 
		/// @param items 
		explicit SimpleTab(const SizeF& tabSize, const Array<String>& items);

		/// @brief すべてのタブを描画します。
		/// @param pos タブ描画開始位置
		/// @param font テキスト描画に使うフォント（FontMethod::MSDF 設定で作成）
		/// @param color アクティブなタブの背景色
		/// @param outlineColor タブの枠の色
		void draw(const Vec2& pos, const Font& font, const ColorF& color, const ColorF& outlineColor) const override;

		/// @brief 
		/// @param index 
		/// @return 
		virtual const RectF getTabRect(size_t index) const noexcept override;
	};
}

#endif // !SIP_ROUND_TAB_H_
