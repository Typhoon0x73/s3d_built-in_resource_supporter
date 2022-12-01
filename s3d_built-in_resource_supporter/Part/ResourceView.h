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

		explicit ResourceView() noexcept;

		~ResourceView() noexcept;

		void update() noexcept;

		void draw() noexcept;

	private:

		RectF regist_button_rect_;

		RectF render_rect_;

		RectF toggle_render_rect_;

		bool* is_only_file_name_;
	};
}

#endif // !SIP_RESOURCE_VIEW_H_
