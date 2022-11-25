#pragma once
#ifndef SIP_TAB_VIEW_H_
#define SIP_TAB_VIEW_H_

#include <Siv3D.hpp>
#include "../Tab/ITab.h"

namespace sip
{
	/// @brief
	class TabView
	{
	public:

		/// @brief 
		/// @param items 
		/// @param pos 
		explicit TabView(const Array<String>& items, const Vec2& pos = { 0, 0 }) noexcept;

		/// @brief 
		~TabView() noexcept;

		/// @brief 
		void update() noexcept;

		/// @brief 
		void draw() const noexcept;

		/// @brief
		/// @param pos
		void setPos(const Vec2& pos) noexcept;

		/// @brief 
		/// @return 
		size_t getSelectTabNo() const noexcept;

		/// @brief
		/// @return 
		const Vec2& getPos() const noexcept;

		/// @brief 
		/// @param index 
		/// @return 
		const RectF getTabRect(size_t index) const noexcept;

	private:

		/// @brief 
		TabPtr tab_;

		/// @brief 
		Vec2 draw_pos_;
	};
}

#endif // !SIP_TAB_VIEW_H_
