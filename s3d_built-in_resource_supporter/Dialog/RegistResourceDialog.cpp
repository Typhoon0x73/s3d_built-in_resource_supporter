#include "RegistResourceDialog.h"
#include "../Command/RegistResourceCommand.h"
#include "../Command/CommandManager.h"
#include "../ResourceInfo/ResourceInfo.h"
#include "../ResourceInfo/ResourceSection.h"
#include "../ResourceInfo/ResourceTag.h"
#include "../XMLInfo/EditFilters.h"
#include "../XMLInfo/XMLData.h"
#include "../XMLInfo/XMLElement.h"
#include "../XMLInfo/XMLAttribute.h"
#include "../ThemeColor/ThemeColorManager.h"
#include "../ToolDefine.h"
#include "../stdafx.h"

namespace sip
{
	Array<String> getFilters()
	{
		auto guids = g_pGetBlackboard(EditFilters* const)->get("edit_filters")->getGUIDs();
		Array<String> filters;
		for (const auto& guid : guids)
		{
			filters.emplace_back(guid.first);
		}
		return filters;
	}
	Array<String> getTags()
	{
		auto select_tab    = g_pGetBlackboard(size_t* const)->get("select_tab_no");
		auto resource_info = g_pGetBlackboard(ResourceInfo* const)->get("resource_info");
		auto section       = resource_info->getSection(section_table[*select_tab]);
		Array<String> tags;
		for (const auto& tag : section->getTags())
		{
			tags.emplace_back(tag->getName());
		}
		return tags;
	}

	RegistResourceDialog::RegistResourceDialog() noexcept
		: result_{ Result::None }
		, edit_{}
		, base_rect_{ Arg::center_<RectF::position_type>(Scene::CenterF()), Scene::Size().x * 0.7, Scene::Size().y * 0.6 }
		, font_{ g_pGetBlackboard(Font* const)->get("gui_font") }
		, tag_names_{ getTags() }
		, filter_names_{ getFilters() }
		, tag_combo_box_{ &tag_names_, font_, { 160, 250 }, { base_rect_.w * 0.7 + 60, 150 } }
		, filter_combo_box_{ &filter_names_, font_, { 160, 320 }, { base_rect_.w * 0.7 + 60, 150 } }
	{
	}

	RegistResourceDialog::~RegistResourceDialog() noexcept
	{
	}

	bool RegistResourceDialog::update() noexcept
	{
		auto command_manager = g_pGetBlackboard(CommandManager* const)->get("command_manager");
		auto resource_info   = g_pGetBlackboard(ResourceInfo*   const)->get("resource_info");
		auto edit_filters    = g_pGetBlackboard(EditFilters*    const)->get("edit_filters");
		if (command_manager == nullptr)
		{
			Logger << U"マネージャーが正常に取得できませんでした。\n";
			result_ = Result::Failed;
			return false;
		}
		if (!tag_combo_box_.isOpen())
		{
			filter_combo_box_.update();
		}
		if (filter_combo_box_.isOpen())
		{
			tag_combo_box_.close();
		}
		tag_combo_box_.update();
		if (tag_combo_box_.isOpen())
		{
			filter_combo_box_.close();
		}
		if (result_ != Result::None)
		{
			return false;
		}
		return true;
	}

	void RegistResourceDialog::draw() const noexcept
	{
		auto col_mng = g_pGetBlackboard(ThemeColorManager* const)->get("theme_color_manager");
		if (col_mng == nullptr)
		{
			return;
		}
		base_rect_.rounded(10)
			.drawShadow({ -5, -5 }, 10.0, 3.0, Palette::Whitesmoke)
			.drawShadow({  5,  5 }, 10.0, 3.0, Palette::Darkgray)
			.draw(col_mng->getMainBackground());

		RectF path_rect{ base_rect_.x, base_rect_.y, base_rect_.w * 0.7, 30 };
		path_rect.movedBy(40, 40).rounded(5)
			.drawShadow({  2,  2 }, 5.0, 0.0, Palette::Whitesmoke)
			.drawShadow({ -2, -2 }, 5.0, 0.0, Palette::Darkgray)
			.draw(col_mng->getMainBackground());

		RectF load_rect{ path_rect.rightX() + 30, base_rect_.y, 30, 30 };
		load_rect.movedBy(40, 40).rounded(5)
			.drawShadow({ -2, -2 }, 5.0, 0.0, Palette::Whitesmoke)
			.drawShadow({  2,  2 }, 5.0, 0.0, Palette::Darkgray)
			.draw(col_mng->getMainBackground());

		tag_combo_box_.draw();
		if (!tag_combo_box_.isOpen())
		{
			filter_combo_box_.draw();
		}
	}

	bool RegistResourceDialog::isModal() const noexcept
	{
		return true;
	}

	bool RegistResourceDialog::isEnableCover() const noexcept
	{
		return true;
	}

	std::int32_t RegistResourceDialog::result() const noexcept
	{
		return static_cast<std::int32_t>(result_);
	}

	void RegistResourceDialog::load_resource() noexcept
	{
		auto resource_path = g_pGetBlackboard(FilePath* const)->get("resource_path");
		if (resource_path == nullptr)
		{
			Logger << U"ファイルパスが正常に取得できませんでした。\n";
			return;
		}
		auto open_file = Dialog::OpenFile();
		if (!open_file.has_value())
		{
			return;
		}
		edit_.path = FileSystem::RelativePath(open_file.value(), FileSystem::ParentPath(*resource_path));
		if (edit_.tag.compare(U"") == 0)
		{
			edit_.tag = FileSystem::Extension(edit_.path).uppercased();
			//tag_list_box_state_.selectedItemIndex = 0;
		}
	}
}
