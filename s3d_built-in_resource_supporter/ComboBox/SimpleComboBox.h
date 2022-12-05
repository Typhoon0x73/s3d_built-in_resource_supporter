#pragma once
#ifndef SIP_SIMPLE_COMBO_BOX_H_
#define SIP_SIMPLE_COMBO_BOX_H_

#include <Siv3D.hpp>

//参考
// https://zenn.dev/reputeless/books/siv3d-documentation/viewer/sample-ui#%E3%83%97%E3%83%AB%E3%83%80%E3%82%A6%E3%83%B3%E3%83%A1%E3%83%8B%E3%83%A5%E3%83%BC

namespace sip
{
	/// @brief 
	class SimpleComboBox
	{
	public:

		/// @brief 
		/// @param items 
		/// @param font 
		/// @param pos 
		explicit SimpleComboBox(const Array<String>* items, Font* font, const Vec2& pos = { 0, 0 }, const SizeF& size = { 0, 0 });

		/// @brief
		/// @return 
		bool isEmpty() const noexcept;

		/// @brief 
		void update();

		/// @brief 
		void draw() const;

		/// @brief 
		/// @param pos 
		void setPos(const Vec2& pos) noexcept;

		/// @brief 
		/// @return 
		const RectF& getRect() const noexcept;

		/// @brief 
		/// @return 
		size_t getSelectIndex() const noexcept;

		/// @brief 
		/// @return 
		String getSelectItem() const noexcept;

		/// @brief 
		void close() noexcept;

		/// @brief 
		/// @return 
		bool isOpen() const noexcept;

	private:

		/// @brief 
		Font* font_;

		/// @brief 
		const Array<String>* items_;

		/// @brief 
		Vec2 scroll_;

		/// @brief 
		Vec2 scroll_max_;

		/// @brief 
		size_t index_;

		/// @brief 
		SizeF padding_;

		/// @brief 
		RectF rect_;

		/// @brief 
		int32 down_button_size_;

		/// @brief 
		bool is_open_;

		/// @brief 
		RectF pulldown_rect_;

		/// @brief 
		RenderTexture pulldown_render_target_;
	};
}

#endif // !SIP_SIMPLE_COMBO_BOX_H_
