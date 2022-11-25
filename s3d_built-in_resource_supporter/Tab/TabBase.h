#pragma once
#ifndef SIP_TAB_BASE_H_
#define SIP_TAB_BASE_H_

#include "ITab.h"

namespace sip
{
	/// @brief 
	class TabBase : public ITab
	{
	public:

		/// @brief 
		/// @param font 
		/// @param pos 
		/// @param tabSize 
		/// @param items 
		explicit TabBase(const Font& font, const Vec2& pos, const SizeF& tabSize, const Array<String>& items);

		/// @brief 
		virtual void update() override;

		/// @brief 
		/// @return 
		virtual const Vec2& getPos() const noexcept override;

		/// @brief タブの個数を返します。
		/// @return タブの個数
		virtual size_t getTabCount() const noexcept override;

		/// @brief アクティブなタブのインデックスを返します。
		/// @return アクティブなタブのインデックス
		virtual size_t getActiveTabIndex() const noexcept override;

		/// @brief アクティブなタブを変更します。
		/// @param index アクティブにするタブのインデックス
		virtual void setActiveTabIndex(size_t index) noexcept override;

		/// @brief 
		/// @param pos 
		/// @return 
		virtual void setPos(const Vec2& pos) noexcept override;

		/// @brief 
		/// @param font 
		/// @return 
		virtual void setFont(const Font& font) noexcept override;

		/// @brief タブを左右に移動します。
		/// @param offset 左に移動する場合 -1, 右に移動する場合は +1
		/// @param wrapAround 端まで到達したときに反対側に戻る場合 true, それ以外の場合は false
		virtual void advance(int32 offset, bool wrapAround = false) override;

	protected:

		/// @brief 
		SizeF tab_size_;

		/// @brief 
		Array<String> items_;

		/// @brief 
		size_t active_index_;

		/// @brief 
		Vec2 pos_;

		/// @brief 
		Font font_;

	};
}

#endif // !SIP_TAB_BASE_H_
