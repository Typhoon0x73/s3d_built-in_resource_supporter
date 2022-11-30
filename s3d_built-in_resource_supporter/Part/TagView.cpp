#include "TagView.h"
#include "../stdafx.h"
#include "../ThemeColor/ThemeColorManager.h"
#include "../ResourceInfo/ResourceInfo.h"
#include "../ResourceInfo/ResourceSection.h"
#include "../ResourceInfo/ResourceTag.h"
#include "../ResourceInfo/ResourceData.h"
#include "../ToolDefine.h"

namespace sip
{
	TagView::TagView(const RectF& render_rect, size_t* tab_no) noexcept
		: render_rect_{ render_rect }
		, render_texture_{ static_cast<int32>(render_rect.w), static_cast<int32>(render_rect.h) }
		, page_list_param_ptr_{ g_pGetBlackboard(TagParams* const)->get("tag_page_list_params") }
		, select_tab_no_ptr_{ tab_no }
	{
	}

	TagView::~TagView() noexcept
	{
	}

	void TagView::update() noexcept
	{
		if (select_tab_no_ptr_ == nullptr)
		{
			return;
		}
		// リソースデータ
		auto resource_info = g_pGetBlackboard(ResourceInfo* const)->get("resource_info");
		if (resource_info == nullptr)
		{
			return;
		}

		const auto  tab_no     = *select_tab_no_ptr_;
		const auto  section_no = section_table[tab_no];
		const auto& font       = SimpleGUI::GetFont();
		auto section = resource_info->getSection(section_no);
		if (section == nullptr)
		{
			return;
		}
		constexpr double padding = 10.0;
		const double item_h = font.fontSize() + padding * 1.5;
		const auto index = section_no - 1;
		PageListParam& param = (*page_list_param_ptr_)[index];
		double offset_y  = padding - param.scroll.y + render_rect_.y;
		const auto& tags = section->getTags();
		for (size_t i = 0; i < tags.size(); ++i)
		{
			auto font_rect = font(tags[i]->getName()).region();
			auto select_rect = font_rect.movedBy(render_rect_.x + padding - param.scroll.x, offset_y);
			select_rect.w = Max(select_rect.w, render_rect_.w - padding * 2.0);
			if (render_rect_.mouseOver() && select_rect.leftClicked())
			{
				param.select_no = i;
				break;
			}
			offset_y += item_h;
			param.page_size.x = Max(param.page_size.x, font_rect.w + padding * 2.0);
		}
		param.page_size.y = tags.size() * item_h + padding * 2.0;
		if (render_rect_.mouseOver())
		{
			if (KeyShift.pressed())
			{
				param.scroll.x += Mouse::Wheel() * 8;
				auto scroll_max = param.page_size.x - render_rect_.w;
				param.scroll.x = Clamp(param.scroll.x, 0.0, Max(scroll_max, 0.0));
			}
			else
			{
				param.scroll.y += Mouse::Wheel() * 8;
				auto scroll_max = param.page_size.y - render_rect_.h;
				param.scroll.y = Clamp(param.scroll.y, 0.0, Max(scroll_max, 0.0));
			}

		}
	}

	void TagView::draw() const noexcept
	{
		auto col_mng = g_pGetBlackboard(ThemeColorManager* const)->get("theme_color_manager");
		if (col_mng == nullptr)
		{
			return;
		}
		// リソースデータ
		auto resource_info = g_pGetBlackboard(ResourceInfo* const)->get("resource_info");
		if (resource_info == nullptr)
		{
			return;
		}

		render_rect_.rounded(5.0)
			.drawShadow({ -3, -3 }, 5.0, 0.0, Palette::Darkgray)
			.drawShadow({ 3,  3 }, 5.0, 0.0, Palette::Whitesmoke)
			.draw(col_mng->getMainBackground());

		const auto  tab_no     = *select_tab_no_ptr_;
		const auto  section_no = section_table[tab_no];
		const auto& font  = SimpleGUI::GetFont();
		const auto  index = section_no - 1;
		const auto& param = (*page_list_param_ptr_)[index];
		constexpr double padding = 10.0;
		const double item_h = font.fontSize() + padding * 1.5;

		auto section = resource_info->getSection(section_no);
		if (section == nullptr)
		{
			return;
		}

		render_texture_.clear(col_mng->getMainBackground());
		{
			ScopedRenderTarget2D target{ render_texture_ };
			double offset_y = padding - param.scroll.y;
			const auto& tags = section->getTags();
			for (size_t i = 0; i < tags.size(); ++i)
			{
				font(tags[i]->getName()).draw(padding - param.scroll.x, offset_y, Palette::Dimgray);
				if (param.select_no.has_value()
					&& i == param.select_no.value())
				{
					auto select_rect = font(tags[i]->getName())
						.region(padding - param.scroll.x, offset_y).stretched(2, 0);
					select_rect.w = Max(select_rect.w, render_rect_.w - padding * 2.0);
					sip::drawDotRect(select_rect);
				}
				offset_y += item_h;
			}
		}
		render_texture_.draw(render_rect_.pos);
	}
}
