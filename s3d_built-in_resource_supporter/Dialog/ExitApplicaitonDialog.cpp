#include "ExitApplicaitonDialog.h"
#include "../stdafx.h"
#include "../ThemeColor/ThemeColorManager.h"

namespace sip
{

	ExitApplicationDialog::ExitApplicationDialog() noexcept
		: result_{ Result::None }
		, base_rect_{ Arg::center_<RectF::position_type>(Scene::CenterF()), SizeF{ Scene::Size() * 0.45 }}
		, cancel_rect_{ base_rect_.rightX() - 30 - 30, base_rect_.bottomY() - 30 - 30, 30, 30 }
		, ok_rect_{ cancel_rect_.x - 30 - 30, cancel_rect_.y, 30, 30 }
		, cancel_texture_{ U"❌"_emoji }
		, ok_texture_{ U"✔️"_emoji }
		, font_{ g_pGetBlackboard(Font* const)->get("gui_font") }
	{
	}

	ExitApplicationDialog::~ExitApplicationDialog() noexcept
	{
	}

	bool ExitApplicationDialog::update() noexcept
	{
		if (KeyEscape.down())
		{
			result_ = Result::Cancel;
		}
		if (ok_rect_.leftClicked())
		{
			result_ = Result::Ok;
			System::Exit();
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

	void ExitApplicationDialog::draw() const noexcept
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

		(*font_)(U"exit application?\n(Any unsaved data will be discarded.)").draw(base_rect_.stretched(-20, -20), Palette::Dimgray);
		
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
		
		
		light_shadow_pos = (ok_rect_.leftPressed() ? Vec2{  2,  2 } : Vec2{ -2, -2 });
		dark_shadow_pos  = (ok_rect_.leftPressed() ? Vec2{ -2, -2 } : Vec2{  2,  2 });
		mouse_over_col   = (ok_rect_.mouseOver()   ? ColorF(Palette::Gainsboro) : col_mng->getMainBackground());
		ok_rect_.rounded(5)
			.drawShadow(light_shadow_pos, 5.0, 0.0, Palette::Whitesmoke)
			.drawShadow(dark_shadow_pos , 5.0, 0.0, Palette::Darkgray)
			.draw(mouse_over_col);
		tex_scale =
			ok_rect_.w / ok_texture_.width();
		ok_texture_.scaled(tex_scale * 0.65).drawAt(ok_rect_.center());
	}

	bool ExitApplicationDialog::isModal() const noexcept
	{
		return true;
	}

	bool ExitApplicationDialog::isEnableCover() const noexcept
	{
		return true;
	}

	std::int32_t ExitApplicationDialog::result() const noexcept
	{
		return static_cast<std::int32_t>(result_);
	}
}
