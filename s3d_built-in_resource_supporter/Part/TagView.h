#pragma once
#ifndef SIP_TAG_VIEW_H_
#define SIP_TAG_VIEW_H_

#include <Siv3D.hpp>
#include "../ToolDefine.h"

namespace sip
{
	/// @brief 
	class TagView
	{
	public:

		/// @brief
		/// @param render_rect 
		/// @param tab_no 
		explicit TagView(const RectF& render_rect) noexcept;

		/// @brief 
		~TagView() noexcept;

		/// @brief 
		void update() noexcept;

		/// @brief 
		void draw() const noexcept;

	private:

		/// @brief 
		RenderTexture render_texture_;

		/// @brief 
		RectF render_rect_;

		/// @brief 
		TagParams* page_list_param_ptr_;

		/// @brief 
		size_t* select_tab_no_ptr_;

		/// @brief 
		Font* gui_font_;
	};
}

#endif // !SIP_TAG_VIEW_H_
