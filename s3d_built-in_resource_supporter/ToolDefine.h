#pragma once
#ifndef SIP_TOOL_DEFINE_H_
#define SIP_TOOL_DEFINE_H_

#include <Siv3D.hpp>
#include "MenuFunc.h"

namespace sip
{
	/// @brief 
	extern const size_t section_table[2];

	/// @brief 
	constexpr const char32_t resource_guidline_url[] = U"https://zenn.dev/reputeless/articles/article-minimum";

	/// @brief 
	constexpr const char32_t title_name[] = U"built-in_resource_supporter";

	/// @brief 
	struct PageListParam
	{
		/// @brief 
		Vec2 scroll{ 0.0, 0.0 };

		/// @brief 
		Vec2 scroll_max{ 0.0, 0.0 };

		/// @brief 
		SizeF page_size{ 0.0, 0.0 };

		/// @brief 
		Optional<size_t> select_no{ none };

		void init()
		{
			scroll     = { 0.0, 0.0 };
			scroll_max = { 0.0, 0.0 };
			page_size  = { 0.0, 0.0 };
			select_no  = none;
		}
	};
	using TagParams      = Array<PageListParam>;
	using ResourceParams = Array<TagParams>;

	/// @brief 
	enum class MenuItemName
	{
		File,
		Edit,
		Help,

		MAX,
	};
	constexpr size_t menu_item_name_count = static_cast<size_t>(MenuItemName::MAX);

	/// @brief 
	struct MenuItem
	{
		/// @brief 
		String name;

		/// @brief 
		Array<String> items;

		/// @brief 
		Array<std::function<bool()>> funcs;

		/// @brief 
		Array<std::function<bool()>> enable_funcs;

	} static const menu_item_table[] =
	{
		{
			U"file",
			{ U"open", U"save", U"close", U"exit" },
			{ MenuFunc::fileOpen, MenuFunc::fileSave, MenuFunc::fileClose, MenuFunc::appEnd },
			{ []() { return true; }, MenuEnableFunc::isOpen, MenuEnableFunc::isOpen, []() { return true; } }
		},

		{
			U"edit",
			{ U"undo", U"redo", U"regist resource", U"erase resource" },
			{ MenuFunc::undo, MenuFunc::redo, MenuFunc::registResource, MenuFunc::eraseResource },
			{ MenuEnableFunc::existExecCommand, MenuEnableFunc::existUndoCommand, MenuEnableFunc::isOpen, MenuEnableFunc::existSelectUserResource }
		},

		{
			U"help",
			{ U"license view on webpage", U"engine resource built-in cancel guidline" },
			{ []() { LicenseManager::ShowInBrowser(); return true; }, []() { System::LaunchBrowser(resource_guidline_url); return true; } },
			{ []() { return true; }, []() { return true; } }
		},
	};

	extern void drawDotRect(const RectF& rect);
	extern RoundRect getToggleEnableRect(const Vec2& pos, double height);
	extern RoundRect drawEnable(bool enable, const Vec2& pos, double height, const ColorF& active_color, const ColorF& disable_color);
}

#endif // !SIP_TOOL_DEFINE_H_
