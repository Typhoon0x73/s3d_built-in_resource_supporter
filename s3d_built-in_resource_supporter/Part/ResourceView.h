#pragma once
#ifndef SIP_RESOURCE_VIEW_H_
#define SIP_RESOURCE_VIEW_H_

#include <Siv3D.hpp>
#include "../ToolDefine.h"

namespace sip
{
	/// @brief 
	class ResourceView
	{
	public:

		/// @brief 
		/// @param rect 
		/// @param tab_no 
		explicit ResourceView(const RectF& rect, size_t* tab_no) noexcept;

		/// @brief 
		~ResourceView() noexcept;

		/// @brief 
		void update() noexcept;

		/// @brief 
		void draw() const noexcept;

	private:

		/// @brief 
		RectF regist_button_rect_;

		/// @brief 
		RectF render_rect_;

		/// @brief 
		RectF toggle_render_rect_;

		/// @brief 
		RenderTexture resource_render_target_;

		/// @brief 
		RenderTexture toggle_render_target_;

		/// @brief 
		ResourceParams* page_list_param_ptr_;

		/// @brief 
		size_t* select_tag_;

		/// @brief 
		bool* is_only_file_name_;
	};
}

#endif // !SIP_RESOURCE_VIEW_H_
