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
		, path_rect_{ base_rect_.x + base_rect_.w * 0.1, base_rect_.y + 40, base_rect_.w * 0.7, 30 }
		, load_rect_{ base_rect_.rightX() - base_rect_.w * 0.1 - 30, base_rect_.y + 40, 30, 30 }
		, cancel_rect_{ load_rect_.x, base_rect_.bottomY() - 30 - 30, 30, 30 }
		, regist_rect_{ cancel_rect_.x - 30 - 30, cancel_rect_.y, 30, 30 }
		, tag_names_{ getTags() }
		, filter_names_{ getFilters() }
		, tag_combo_box_{ &tag_names_, font_, { path_rect_.x, 250 }, { base_rect_.w * 0.8, 150 } }
		, filter_combo_box_{ &filter_names_, font_, { path_rect_.x, 320 }, { base_rect_.w * 0.8, 150 } }
		, open_file_texture_{ U"📂"_emoji }
		, cancel_texture_{ U"❌"_emoji }
		, regist_texture_{ U"✔️"_emoji }
	{
		for (size_t i : step(filter_names_.size()))
		{
			if (filter_names_[i].compare(edit_.filter) == 0)
			{
				filter_combo_box_.setIndex(i);
				break;
			}
		}
		if (tag_names_.size() > 0)
		{
			edit_.tag = tag_names_[0];
		}
	}

	RegistResourceDialog::~RegistResourceDialog() noexcept
	{
	}

	bool RegistResourceDialog::update() noexcept
	{
		auto command_manager = g_pGetBlackboard(CommandManager* const)->get("command_manager");
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
		if (not tag_combo_box_.isEmpty())
		{
			edit_.tag = tag_combo_box_.getSelectItem();
		}
		if (not filter_combo_box_.isEmpty())
		{
			edit_.filter = filter_combo_box_.getSelectItem();
		}
		if (load_rect_.leftClicked())
		{
			tag_combo_box_.close();
			filter_combo_box_.close();
			load_resource();
		}
		if (regist_rect_.leftClicked())
		{
			RegistResourceCommand::CreateInfo info;
			info.path       = edit_.path;
			info.tag        = edit_.tag;
			info.section    = 2;
			info.filter     = edit_.filter;
			info.extensions = edit_.extensions;
			if (command_manager->regist(std::move(std::make_unique<RegistResourceCommand>(info))))
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

		path_rect_.rounded(5)
			.drawShadow({  2,  2 }, 5.0, 0.0, Palette::Whitesmoke)
			.drawShadow({ -2, -2 }, 5.0, 0.0, Palette::Darkgray)
			.draw(col_mng->getMainBackground());
		(*font_)(edit_.path).draw(path_rect_.stretched({ -10, 0 }), Palette::Dimgray);
		
		Vec2 light_shadow_pos = (load_rect_.leftPressed() ? Vec2{  2,  2 } : Vec2{ -2, -2 });
		Vec2 dark_shadow_pos  = (load_rect_.leftPressed() ? Vec2{ -2, -2 } : Vec2{  2,  2 });
		ColorF mouse_over_col = (load_rect_.mouseOver()   ? ColorF(Palette::Gainsboro) : col_mng->getMainBackground());
		load_rect_.rounded(5)
			.drawShadow(light_shadow_pos, 5.0, 0.0, Palette::Whitesmoke)
			.drawShadow(dark_shadow_pos , 5.0, 0.0, Palette::Darkgray)
			.draw(mouse_over_col);

		auto tex_scale =
			load_rect_.w / open_file_texture_.width();
		open_file_texture_.scaled(tex_scale * 0.65).drawAt(load_rect_.center());

		light_shadow_pos = (cancel_rect_.leftPressed() ? Vec2{  2,  2 } : Vec2{ -2, -2 });
		dark_shadow_pos  = (cancel_rect_.leftPressed() ? Vec2{ -2, -2 } : Vec2{  2,  2 });
		mouse_over_col   = (cancel_rect_.mouseOver()   ? ColorF(Palette::Gainsboro) : col_mng->getMainBackground());
		cancel_rect_.rounded(5)
			.drawShadow(light_shadow_pos, 5.0, 0.0, Palette::Whitesmoke)
			.drawShadow(dark_shadow_pos , 5.0, 0.0, Palette::Darkgray)
			.draw(mouse_over_col);
		tex_scale =
			cancel_rect_.w / cancel_texture_.width();
		cancel_texture_.scaled(tex_scale * 0.65).drawAt(load_rect_.center());
		
		
		light_shadow_pos = (regist_rect_.leftPressed() ? Vec2{  2,  2 } : Vec2{ -2, -2 });
		dark_shadow_pos  = (regist_rect_.leftPressed() ? Vec2{ -2, -2 } : Vec2{  2,  2 });
		mouse_over_col   = (regist_rect_.mouseOver()   ? ColorF(Palette::Gainsboro) : col_mng->getMainBackground());
		regist_rect_.rounded(5)
			.drawShadow(light_shadow_pos, 5.0, 0.0, Palette::Whitesmoke)
			.drawShadow(dark_shadow_pos , 5.0, 0.0, Palette::Darkgray)
			.draw(mouse_over_col);
		tex_scale =
			regist_rect_.w / regist_texture_.width();
		regist_texture_.scaled(tex_scale * 0.65).drawAt(load_rect_.center());

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
		auto tmp_tag = FileSystem::Extension(edit_.path).uppercased();
		if (edit_.tag.compare(U"") == 0)
		{
			edit_.tag = tmp_tag;
		}
		for (size_t i : step(tag_names_.size()))
		{
			if (i != 0 && tag_names_[i].compare(tmp_tag) == 0)
			{
				std::swap(tag_names_[0], tag_names_[i]);
			}
		}
		if (tag_names_.size() <= 0 || tag_names_[0].compare(tmp_tag) != 0)
		{
			tag_names_.push_front(tmp_tag);
		}
	}
}
