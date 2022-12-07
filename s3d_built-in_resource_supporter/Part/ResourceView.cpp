#include "../stdafx.h"
#include "ResourceView.h"
#include "../ResourceInfo/ResourceInfo.h"
#include "../ResourceInfo/ResourceSection.h"
#include "../ResourceInfo/ResourceTag.h"
#include "../ResourceInfo/ResourceData.h"
#include "../Command/ToggleEnableCommand.h"
#include "../Command/CommandManager.h"
#include "../ThemeColor/ThemeColorManager.h"

namespace sip
{
	ResourceView::ResourceView(const RectF& rect) noexcept
		: render_rect_{ rect }
		, regist_button_rect_{ 0, 0, 220, 30 }
		, erase_button_rect_{ 0, 0, 30, 30 }
		, erase_texture_{ Texture{ U"🗑️"_emoji } }
		, toggle_render_rect_{ render_rect_.stretched(0, -330, 0, -10) }
		, select_tab_{ g_pGetBlackboard(size_t* const)->get("select_tab_no") }
		, is_only_file_name_{ g_pGetBlackboard(bool* const)->get("is_only_file_name") }
		, tag_page_list_param_ptr_{ g_pGetBlackboard(TagParams* const)->get("tag_page_list_params") }
		, page_list_param_ptr_{ g_pGetBlackboard(ResourceParams* const)->get("resource_page_list_params") }
		, resource_render_target_{ static_cast<int32>(render_rect_.w), static_cast<int32>(render_rect_.h) }
		, font_{ g_pGetBlackboard(Font* const)->get("gui_font") }
	{
	}

	ResourceView::~ResourceView() noexcept
	{
	}

	void ResourceView::update() noexcept
	{
		// リソースデータ
		auto resource_info = g_pGetBlackboard(ResourceInfo* const)->get("resource_info");
		auto cmd_mng       = g_pGetBlackboard(CommandManager* const)->get("command_manager");
		// リソースの選択
		if (resource_info == nullptr
			|| cmd_mng == nullptr)
		{
			return;
		}
		const auto  tab_no     = *select_tab_;
		const auto  section_no = section_table[tab_no];
		const auto  tag_no     = (*tag_page_list_param_ptr_)[tab_no].select_no;
		const auto& section    = resource_info->getSection(section_no);
		auto line_y = render_rect_.h - (regist_button_rect_.h + 20);
		if (tab_no == 0 && MenuEnableFunc::isOpen())
		{
			auto button_rect = regist_button_rect_
				.movedBy((render_rect_.w - regist_button_rect_.w) * 0.5, line_y + 10.0)
				.movedBy(render_rect_.pos);
			if (button_rect.leftClicked())
			{
				if (!MenuFunc::registResource())
				{

				}
			}
		}
		if (!tag_no)
		{
			return;
		}
		if (section == nullptr)
		{
			return;
		}
		auto& page_param = (*page_list_param_ptr_)[tab_no][tag_no.value()];
		auto& scroll     = page_param.scroll;
		double offset_y  = 10 - scroll.y + render_rect_.y;
		if (tab_no == 0 && page_param.select_no.has_value())
		{
			auto button_rect = erase_button_rect_
				.movedBy((render_rect_.w + regist_button_rect_.w) * 0.5 + 10.0, line_y + 10.0)
				.movedBy(render_rect_.pos);
			if (button_rect.leftClicked())
			{
				if (!MenuFunc::eraseResource())
				{

				}
			}
		}
		if (const auto& tag = section->getTag(tag_no.value()))
		{
			page_param.page_size.x = 0;
			RectF regist_area{ render_rect_.x, render_rect_.y + line_y, render_rect_.size };
			const auto& resources = tag->getResources();
			for (size_t i = 0; i < resources.size(); ++i)
			{
				auto toggle_rect = getToggleEnableRect(Vec2{ 10, offset_y + 6 }, 24).movedBy(render_rect_.x, 0);
				if (toggle_rect.leftClicked() && !regist_area.mouseOver())
				{
					ToggleEnableCommand::CreateInfo info;
					info.section = section_no;
					info.tag     = tag->getName();
					info.index   = i;
					if (!cmd_mng->regist(std::move(std::make_unique<ToggleEnableCommand>(info))))
					{

					}
				}
				FilePath path{ resources[i]->getPath() };
				if (*is_only_file_name_)
				{
					path = FileSystem::FileName(FileSystem::FullPath(path));
				}
				auto select_rect = (*font_)(path)
					.region(render_rect_.x + 65 - scroll.x, offset_y);
				select_rect.w = render_rect_.w - 70;
				if (select_rect.leftClicked() && !regist_area.mouseOver())
				{
					page_param.select_no = i;
					break;
				}
				offset_y += 35;
				page_param.page_size.x  = Max(page_param.page_size.x, (*font_)(path).region().w + 85);
				page_param.scroll_max.x = page_param.page_size.x - render_rect_.w;
			}
			page_param.page_size.y  = resources.size() * 35 + 20 + regist_button_rect_.h + 20;
			page_param.scroll_max.y = page_param.page_size.y - render_rect_.h;
		}
		if (render_rect_.mouseOver())
		{
			if (KeyShift.pressed())
			{
				page_param.scroll.x += Mouse::Wheel() * 8;
			}
			else
			{
				page_param.scroll.y += Mouse::Wheel() * 8;
			}
		}
		page_param.scroll.x = Clamp(page_param.scroll.x, 0.0, Max(page_param.scroll_max.x, 0.0));
		page_param.scroll.y = Clamp(page_param.scroll.y, 0.0, Max(page_param.scroll_max.y, 0.0));
		
	}

	void ResourceView::draw() const noexcept
	{
		// リソースデータ
		auto resource_info = g_pGetBlackboard(ResourceInfo* const)->get("resource_info");
		auto col_mng       = g_pGetBlackboard(ThemeColorManager* const)->get("theme_color_manager");
		if (resource_info == nullptr
			|| col_mng == nullptr)
		{
			return;
		}
		resource_render_target_.clear(col_mng->getMainBackground());
		{
			ScopedRenderTarget2D target{ resource_render_target_ };
			const auto  tab_no     = *select_tab_;
			const auto  section_no = section_table[tab_no];
			const auto& tag_no     = (*tag_page_list_param_ptr_)[tab_no].select_no;
			const auto& section    = resource_info->getSection(section_no);
			if (tag_no && section)
			{
				const auto& page_param = (*page_list_param_ptr_)[tab_no][tag_no.value()];
				const auto& scroll     = page_param.scroll;
				double offset_y        = 10 - scroll.y;
				const auto& tag        = section->getTag(tag_no.value());
				const auto& resources  = tag->getResources();
				for (size_t i = 0; i < resources.size(); ++i)
				{
					FilePath path{ resources[i]->getPath() };
					if (*is_only_file_name_)
					{
						path = FileSystem::FileName(FileSystem::FullPath(path));
					}
					(*font_)(path).draw(65 - scroll.x, offset_y, Palette::Dimgray);
					if (i == page_param.select_no)
					{
						auto select_rect = (*font_)(path)
							.region(65 - scroll.x, offset_y).stretched(2, 0);
						select_rect.w = Max(select_rect.w, render_rect_.w - 70);
						sip::drawDotRect(select_rect);
					}
					offset_y += 35;
				}
				auto draw_toggle_bg = toggle_render_rect_;
				draw_toggle_bg.setPos(0, 0);
				draw_toggle_bg.draw(col_mng->getMainBackground());
				for (size_t i = 0; i < resources.size(); ++i)
				{
					bool enable = resources[i]->isEnable();
					drawEnable(enable, Vec2{ 10, 10 - scroll.y + (i * 35) + 6}, 24, Palette::Royalblue, Palette::Silver);
				}
			}
			if (tab_no == 0 && MenuEnableFunc::isOpen())
			{
				auto line_y = render_rect_.h - (regist_button_rect_.h + 20);
				RectF line_rect{ 0, line_y, render_rect_.size };
				line_rect
					.drawShadow({ 0, -2 }, 5.0, 0.0, Palette::Whitesmoke)
					.draw(col_mng->getMainBackground());
				auto button_color =
					(regist_button_rect_
						.movedBy((render_rect_.w - regist_button_rect_.w) * 0.5, line_y + 10.0)
						.movedBy(render_rect_.pos).mouseOver()
					? ColorF{ Palette::Gainsboro }
				: col_mng->getMainBackground());
				auto draw_regist_region = regist_button_rect_
					.movedBy((render_rect_.w - regist_button_rect_.w) * 0.5, line_y + 10.0)
					.rounded(5.0)
					.drawShadow({  3,  3 }, 5.0, 0.0, Palette::Darkgray)
					.drawShadow({ -3, -3 }, 5.0, 0.0, Palette::Whitesmoke)
					.draw(button_color);
				SimpleGUI::GetFont()(U"regist resource").drawAt(draw_regist_region.center(), Palette::Dimgray);
			}
		}
		render_rect_.rounded(5.0)
			.drawShadow({ -3, -3 }, 5.0, 0.0, Palette::Darkgray)
			.drawShadow({  3,  3 }, 5.0, 0.0, Palette::Whitesmoke)
			.draw(col_mng->getMainBackground());
		resource_render_target_.draw(render_rect_.pos);
	}
}
