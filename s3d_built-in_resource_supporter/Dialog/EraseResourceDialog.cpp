#include "EraseResourceDialog.h"
#include "../stdafx.h"
#include "../ThemeColor/ThemeColorManager.h"
#include "../Command/CommandManager.h"
#include "../Command/EraseResourceCommand.h"
#include "../ResourceInfo/ResourceInfo.h"
#include "../ResourceInfo/ResourceSection.h"
#include "../ResourceInfo/ResourceTag.h"
#include "../ResourceInfo/ResourceData.h"
#include "../XMLInfo/EditFilters.h"
#include "../XMLInfo/XMLData.h"
#include "../XMLInfo/XMLElement.h"
#include "../ToolDefine.h"

namespace sip
{
	EraseResourceDialog::EraseResourceDialog() noexcept
		: font_{ g_pGetBlackboard(Font* const)->get("gui_font") }
		, base_rect_{ Arg::center_<RectF::position_type>(Scene::CenterF()), SizeF{ Scene::Size() * 0.5 }}
		, result_{ Result::None }
		, cancel_rect_{ base_rect_.rightX() - 30 - 30, base_rect_.bottomY() - 30 - 30, 30, 30 }
		, erase_rect_{ cancel_rect_.x - 30 - 30, cancel_rect_.y, 30, 30 }
		, cancel_texture_{ U"❌"_emoji }
		, erase_texture_{ U"✔️"_emoji }
	{
	}

	EraseResourceDialog::~EraseResourceDialog() noexcept
	{
	}

	bool EraseResourceDialog::update() noexcept
	{
		if (KeyEscape.down())
		{
			result_ = Result::Cancel;
		}
		if (erase_rect_.leftClicked())
		{
			auto cmd_mng = g_pGetBlackboard(CommandManager* const)->get("command_manager");
			auto tab_no  = g_pGetBlackboard(size_t* const)->get("select_tab_no");
			auto resource_info   = g_pGetBlackboard(ResourceInfo* const)->get("resource_info");
			auto edit_filters    = g_pGetBlackboard(EditFilters* const)->get("edit_filters");
			auto tag_params      = g_pGetBlackboard(TagParams* const)->get("tag_page_list_params");
			auto resource_params = g_pGetBlackboard(ResourceParams* const)->get("resource_page_list_params");
			auto section = resource_info->getSection(section_table[*tab_no]);
			auto tag_no = (*tag_params)[*tab_no].select_no;
			auto tag = section->getTag(tag_no.value());
			auto resource = tag->getResource((*resource_params)[*tab_no][tag_no.value()].select_no.value());
			auto filter_data = edit_filters->findItemGroup(tag->getName());
			String filter_str = U"";
			for (const auto& child : filter_data->getChildren())
			{
				if (child.first.compare(U"Filter") != 0)
				{
					continue;
				}
				auto element = child.second->findElement(U"Filter");
				filter_str = element->getValue();
				break;
			}
			String ext_str = U"";
			auto ext_data = edit_filters->findFilter(filter_str);
			for (const auto& child : ext_data->getChildren())
			{
				if (child.second->getElement()->getTag().compare(U"Extensions") != 0)
				{
					continue;
				}
				ext_str = child.second->getElement()->getValue();
				break;
			}
			EraseResourceCommand::CreateInfo info;
			info.section = section_table[*tab_no];
			info.tag     = tag->getName();
			info.path    = resource->getPath();
			info.filter  = filter_str;
			info.extensions = ext_str;
			if (cmd_mng->regist(std::move(std::make_unique<EraseResourceCommand>(info))))
			{
				result_ = Result::Success;
			}
		}
		if (cancel_rect_.leftClicked())
		{
			result_ = Result::Cancel;
		}
		if (result_ != Result::None)
		{
			return false;
		}
		return true;
	}

	void EraseResourceDialog::draw() const noexcept
	{
		auto col_mng         = g_pGetBlackboard(ThemeColorManager* const)->get("theme_color_manager");
		auto tab_no          = g_pGetBlackboard(size_t* const)->get("select_tab_no");
		auto resource_info   = g_pGetBlackboard(ResourceInfo* const)->get("resource_info");
		auto tag_params      = g_pGetBlackboard(TagParams* const)->get("tag_page_list_params");
		auto resource_params = g_pGetBlackboard(ResourceParams* const)->get("resource_page_list_params");
		if (col_mng            == nullptr
			|| tab_no          == nullptr
			|| resource_info   == nullptr
			|| tag_params      == nullptr
			|| resource_params == nullptr
			)
		{
			return;
		}
		auto section = resource_info->getSection(section_table[*tab_no]);
		if(section == nullptr)
		{
			return;
		}
		auto tag_no = (*tag_params)[*tab_no].select_no;
		if (not tag_no.has_value())
		{
			return;
		}
		auto tag = section->getTag(tag_no.value());
		if (tag == nullptr)
		{
			return;
		}
		auto res_no = (*resource_params)[*tab_no][tag_no.value()].select_no;
		if (not res_no.has_value())
		{
			return;
		}
		auto resource = tag->getResource(res_no.value());
		if (resource == nullptr)
		{
			return;
		}
		base_rect_.rounded(10)
			.drawShadow({ -5, -5 }, 10.0, 3.0, Palette::Whitesmoke)
			.drawShadow({  5,  5 }, 10.0, 3.0, Palette::Darkgray)
			.draw(col_mng->getMainBackground());
		(*font_)(U"delete resource:[{}].\nAre you sure?"_fmt(resource->getPath())).draw(base_rect_.stretched(-20, -20), Palette::Dimgray);
		
		auto light_shadow_pos = (cancel_rect_.leftPressed() ? Vec2{  2,  2 } : Vec2{ -2, -2 });
		auto dark_shadow_pos  = (cancel_rect_.leftPressed() ? Vec2{ -2, -2 } : Vec2{  2,  2 });
		auto mouse_over_col   = (cancel_rect_.mouseOver()   ? ColorF(Palette::Gainsboro) : col_mng->getMainBackground());
		cancel_rect_.rounded(5)
			.drawShadow(light_shadow_pos, 5.0, 0.0, Palette::Whitesmoke)
			.drawShadow(dark_shadow_pos , 5.0, 0.0, Palette::Darkgray)
			.draw(mouse_over_col);
		auto tex_scale =
			cancel_rect_.w / cancel_texture_.width();
		cancel_texture_.scaled(tex_scale * 0.65).drawAt(cancel_rect_.center());
		
		
		light_shadow_pos = (erase_rect_.leftPressed() ? Vec2{  2,  2 } : Vec2{ -2, -2 });
		dark_shadow_pos  = (erase_rect_.leftPressed() ? Vec2{ -2, -2 } : Vec2{  2,  2 });
		mouse_over_col   = (erase_rect_.mouseOver()   ? ColorF(Palette::Gainsboro) : col_mng->getMainBackground());
		erase_rect_.rounded(5)
			.drawShadow(light_shadow_pos, 5.0, 0.0, Palette::Whitesmoke)
			.drawShadow(dark_shadow_pos , 5.0, 0.0, Palette::Darkgray)
			.draw(mouse_over_col);
		tex_scale =
			erase_rect_.w / erase_texture_.width();
		erase_texture_.scaled(tex_scale * 0.65).drawAt(erase_rect_.center());
	}

	bool EraseResourceDialog::isModal() const noexcept
	{
		return true;
	}

	bool EraseResourceDialog::isEnableCover() const noexcept
	{
		return true;
	}

	std::int32_t EraseResourceDialog::result() const noexcept
	{
		return static_cast<std::int32_t>(result_);
	}
}
