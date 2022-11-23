#pragma once
#ifndef SIP_ITAB_H_
#define SIP_ITAB_H_

#include <Siv3D.hpp>

// 参考 https://github.com/Siv3D/Siv3D-Samples/blob/main/Samples/TabSample/Main.cpp

namespace sip
{
	/// @brief 
	class ITab
	{
	public:

		/// @brief 
		virtual ~ITab() = default;

		/// @brief すべてのタブを描画します。
		/// @param pos タブ描画開始位置
		/// @param font テキスト描画に使うフォント（FontMethod::MSDF 設定で作成）
		/// @param color アクティブなタブの背景色
		/// @param outlineColor タブの枠の色
		virtual void draw(const Vec2& pos, const Font& font, const ColorF& color, const ColorF& outlineColor) const = 0;

		/// @brief タブの個数を返します。
		/// @return タブの個数
		virtual size_t getTabCount() const noexcept = 0;

		/// @brief アクティブなタブのインデックスを返します。
		/// @return アクティブなタブのインデックス
		virtual size_t getActiveTabIndex() const noexcept = 0;

		/// @brief アクティブなタブを変更します。
		/// @param index アクティブにするタブのインデックス
		virtual void setActiveTabIndex(size_t index) noexcept = 0;

		/// @brief タブを左右に移動します。
		/// @param offset 左に移動する場合 -1, 右に移動する場合は +1
		/// @param wrapAround 端まで到達したときに反対側に戻る場合 true, それ以外の場合は false
		virtual void advance(int32 offset, bool wrapAround = false) = 0;

		/// @brief 
		/// @param index 
		/// @return 
		virtual const RectF getTabRect(size_t index) const noexcept = 0;
	};

	using TabPtr = std::unique_ptr<ITab>;
}

#endif // !SIP_ITAB_H_
