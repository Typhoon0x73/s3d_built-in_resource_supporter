#pragma once
#ifndef SIP_TAG_VIEW_H_
#define SIP_TAG_VIEW_H_

#include <Siv3D.hpp>

namespace sip
{
	/// @brief 
	class TagView
	{
	public:

		/// @brief 
		struct PageParam
		{
			/// @brief 
			double scroll{ 0.0 };

			/// @brief 
			SizeF page_size{ 0.0, 0.0 };

			/// @brief 
			Optional<size_t> select_no{ none };
		};

	public:

		/// @brief 
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
		PageParam page_params_[2];

		/// @brief 
		size_t* select_tab_no_ptr_;

	};
}

#endif // !SIP_TAG_VIEW_H_
