﻿# include <Siv3D.hpp> // OpenSiv3D v0.6.6
# include "ApplicationData.h"
# include "SettingsINI.h"
# include "WindowInfo.h"
# include "Command/CommandManager.h"
# include "ThemeColor/ThemeColorManager.h"
# include "Dialog/DialogManager.h"
# include "MenuFunc.h"
# include "Tab/SimpleTab.h"
# include "ResourceInfo/ResourceInfo.h"
# include "ResourceInfo/ResourceSection.h"
# include "ResourceInfo/ResourceTag.h"
# include "ResourceInfo/ResourceData.h"

using namespace sip;

namespace sip
{
	/// @brief 
	struct MenuItem
	{
		/// @brief 
		String name;

		/// @brief 
		Array<String> items;

		/// @brief 
		Array<std::function<bool()>> funcs;

	} static const menu_item_table[] =
	{
		{
			U"file",
			{ U"open", U"save", U"close" },
			{ MenuFunc::fileOpen, MenuFunc::fileSave, MenuFunc::fileClose }
		},

		{
			U"edit",
			{ U"undo", U"redo" },
			{ MenuFunc::undo, MenuFunc::redo }
		},

		{
			U"view",
			{},
			{}
		},
	};

	void drawDotRect(const RectF& rect)
	{
		constexpr double thickness = 2.0;
		static double offset = 0.0;
		offset += Scene::DeltaTime() * 10;
		rect.top().draw(LineStyle::SquareDot(offset), thickness);
		rect.right().draw(LineStyle::SquareDot(offset), thickness);
		rect.bottom().draw(LineStyle::SquareDot(offset), thickness);
		rect.left().draw(LineStyle::SquareDot(offset), thickness);
	}

	void drawEnable(bool enable, const Vec2& pos, double height, const ColorF& active_color, const ColorF& disable_color)
	{
		Circle circle{ { 0.0, 0.0 }, height * 0.4 };
		RectF rect{ pos, height * 1.5, height };
		ColorF color = (enable ? active_color : disable_color);
		rect.rounded(height * 0.5)
			.draw(color)
			.drawFrame(2.0, 0.0, ColorF{ 0.25, 0.25 }, ColorF{ 0.75, 0.75 })
			;
		circle.setPos(rect.center() + Vec2{ height * 0.25 * (enable ? -1 : 1), 0});
		circle.draw(Palette::Gainsboro);
	}
}

void Main()
{
	// アプリケーションデータの作成
	ApplicationData app_data;

	// ウィンドウサイズ、モードを変更
	//Scene::SetResizeMode(ResizeMode::Actual);
	//Window::SetStyle(WindowStyle::Sizable);
	
	// 設定情報読み込み
	if (!LoadSettingsINI(U"settings.ini"))
	{
		// 初回起動の可能性があるのでウィンドウサイズ変更
		auto window_info = g_pGetBlackboard(WindowInfo* const)->get("window_info");
		if (window_info)
		{
			Window::Resize(window_info->default_size_);
		}
	}

	// 各マネージャー
	auto cmd_mng = g_pGetBlackboard(CommandManager*    const)->get("command_manager");
	auto col_mng = g_pGetBlackboard(ThemeColorManager* const)->get("theme_color_manager");
	auto dlg_mng = g_pGetBlackboard(DialogManager*     const)->get("dialog_manager");

	// リソースデータ
	auto resource_info = g_pGetBlackboard(ResourceInfo* const)->get("resource_info");

	// 背景色の変更
	Scene::SetBackground(col_mng->getMainBackground());

	// メニューバーの作成
	SimpleMenuBar menubar
	{
		{
			{ menu_item_table[0].name, menu_item_table[0].items },
			{ menu_item_table[1].name, menu_item_table[1].items },
			{ menu_item_table[2].name, menu_item_table[2].items },
		}
	};

	// 読み込み用ボタン矩形
	RectF   open_file_rect{ 680, 60, 30, 30 };
	Texture open_file_texture{ U"📂"_emoji };

	// タブ
	Array<String> tab_items = { U"User", U"Engine" };
	TabPtr tab = std::make_unique<SimpleTab>(Size{ 100, 30 }, tab_items);
	Vec2   tab_draw_pos = Vec2{ 100, 110 };
	constexpr size_t section_table[] = { 2, 1 };

	// タグ
	Optional<size_t> select_tag_no[] = { none, none };
	RectF tag_render_rect{
		tab_draw_pos.x, tab_draw_pos.y + tab->getTabRect(0).h + 10,
		200, 400
	};
	Vec2 tag_scroll[] = { { 0.0, 0.0 }, { 0.0, 0.0 } };
	RenderTexture tag_render_target(tag_render_rect.w, tag_render_rect.h);
	SizeF tag_page_size[] = { { 0.0, 0.0 }, { 0.0, 0.0 } };

	// リソース
	Optional<size_t> select_resource_no[] = { none, none };
	RectF regist_button_rect{ 0, 0, 100, 30 };
	RectF resource_render_rect{
		tag_render_rect.rightX() + 10, tag_render_rect.y,
		400, tag_render_rect.h
	};
	Vec2 resource_scroll[] = { { 0.0, 0.0 }, { 0.0, 0.0 } };
	RenderTexture resource_render_target(resource_render_rect.w, resource_render_rect.h);
	SizeF resource_page_size[] = { { 0.0, 0.0 }, { 0.0, 0.0 } };

	// ゲームループ
	while (System::Update())
	{
		// Update
		do {
			// ダイアログの更新
			if (!dlg_mng->update())
			{
				Logger << U"exit dialog. result : " << dlg_mng->result();
				Mouse::ClearLRInput();
				break;
			}
			// ダイアログがある間はほかの操作を受け付けない
			if (dlg_mng->existDialog())
			{
				Mouse::ClearLRInput();
				break;
			}
			// メニューバーの更新
			if (auto select_menu_item = menubar.update())
			{
				const auto& value = select_menu_item.value();
				const auto menu_index = value.menuIndex;
				const auto item_index = value.itemIndex;
				if (!menu_item_table[menu_index].funcs[item_index]())
				{

				}
			}

			// Ctrl + Z で元に戻すコマンド実行
			if (KeyControl.pressed() && KeyZ.down())
			{
				if (!MenuFunc::undo())
				{

				}
			}
			// Ctrl + Y でやり直しコマンド実行
			if (KeyControl.pressed() && KeyY.down())
			{
				if (!MenuFunc::redo())
				{

				}
			}

			// 読み込み矩形選択でファイル読み込み
			if (open_file_rect.mouseOver() && open_file_rect.leftClicked())
			{
				if (!MenuFunc::fileOpen())
				{

				}
			}

			// タブ操作
			for (int32 i = 0; i < tab->getTabCount(); ++i)
			{
				if (tab->getTabRect(i).movedBy(tab_draw_pos).leftClicked())
				{
					tab->setActiveTabIndex(i);
					break;
				}
			}

			// タグの選択
			if (resource_info)
			{
				const auto tab_no     = tab->getActiveTabIndex();
				const auto section_no = section_table[tab_no];
				const auto& font = SimpleGUI::GetFont();
				if (auto section = resource_info->getSection(section_no))
				{
					double offset_y = 10 - tag_scroll[section_no - 1].y + tag_render_rect.y;
					const auto& tags = section->getTags();
					for (size_t i = 0; i < tags.size(); ++i)
					{
						auto select_rect = font(tags[i]->getName())
							.region(tag_render_rect.x + 10, offset_y);
						select_rect.w = tag_render_rect.w - 20;
						if (select_rect.leftClicked())
						{
							select_tag_no[section_no - 1] = i;
							select_resource_no[section_no - 1] = none;
							break;
						}
						offset_y += 35;
					}
					tag_page_size[section_no - 1].y = tags.size() * 35 + 20;
				}
				if (tag_render_rect.mouseOver())
				{
					tag_scroll[section_no - 1].y += Mouse::Wheel() * 8;

					auto scroll_max = tag_page_size[section_no - 1].y - tag_render_rect.h;
					tag_scroll[section_no - 1].y = Clamp(tag_scroll[section_no - 1].y, 0.0, Max(scroll_max, 0.0));
				}
			}

			// リソースの選択
			if (resource_info)
			{
				const auto tab_no     = tab->getActiveTabIndex();
				const auto section_no = section_table[tab_no];
				const auto tag_no     = select_tag_no[section_no - 1];
				const auto& font = SimpleGUI::GetFont();
				const auto& section = resource_info->getSection(section_no);
				if (tag_no && section)
				{
					double offset_y = 10 - resource_scroll[section_no - 1].y + resource_render_rect.y;
					if (const auto& tag = section->getTag(tag_no.value()))
					{
						const auto& resources = tag->getResources();
						for (size_t i = 0; i < resources.size(); ++i)
						{
							auto select_rect = font(resources[i]->getPath())
								.region(resource_render_rect.x + 65, offset_y);
							select_rect.w = resource_render_rect.w - 20;
							if (select_rect.leftClicked())
							{
								select_resource_no[section_no - 1] = i;
								break;
							}
							offset_y += 35;
						}
						resource_page_size[section_no - 1].y = resources.size() * 35 + 20;
					}
				}
				if (resource_render_rect.mouseOver())
				{
					resource_scroll[section_no - 1].y += Mouse::Wheel() * 8;

					auto scroll_max = resource_page_size[section_no - 1].y - resource_render_rect.h;
					resource_scroll[section_no - 1].y = Clamp(resource_scroll[section_no - 1].y, 0.0, Max(scroll_max, 0.0));
				}
			}

		} while (false);

		// Draw
		do {

			// パス用矩形表示
			RectF path_rect{ 150, 60, 500, 30 };
			path_rect.rounded(5.0)
				.drawShadow({  2,  2 }, 5.0, 0.0, Palette::Whitesmoke)
				.drawShadow({ -2, -2 }, 5.0, 0.0, Palette::Darkgray)
				.draw(col_mng->getMainBackground());

			// パスがあれば表示する
			if (auto vcxproj_path = g_pGetBlackboard(FilePath* const)->get("vcxproj_path"))
			{
				auto font = SimpleGUI::GetFont();
				// 矩形内にファイル名を表示
				font(FileSystem::FileName(*vcxproj_path))
					.draw(path_rect.stretched({ -10, 0 }), Palette::Dimgray);
				if (path_rect.mouseOver())
				{
					// マウスオーバーで矩形上部にフルパス表示
					font(*vcxproj_path)
						.draw(
							font.fontSize() * 0.5,
							path_rect.pos - Vec2{0, 15},
							Palette::Dimgray
						);
				}
			}

			// ファイル読み込みボタン描画
			{
				const auto base_col =
					(open_file_rect.mouseOver()
						? ColorF(Palette::Gainsboro)
						: col_mng->getMainBackground());
				open_file_rect.rounded(5.0)
					.drawShadow({ -2, -2 }, 5.0, 0.0, Palette::Whitesmoke)
					.drawShadow({  2,  2 }, 5.0, 0.0, Palette::Darkgray)
					.draw(base_col);
				const auto tex_scale =
					open_file_rect.w / open_file_texture.width();
				open_file_texture.scaled(tex_scale * 0.65).drawAt(open_file_rect.center());
			}

			// タブの描画
			{
				tab->draw(
					tab_draw_pos,
					SimpleGUI::GetFont(),
					col_mng->getMainBackground(),
					ColorF{}
				);
			}

			// タグの描画
			if (resource_info)
			{
				tag_render_target.clear(col_mng->getMainBackground());
				{
					ScopedRenderTarget2D target{ tag_render_target };
					const size_t tab_no     = tab->getActiveTabIndex();
					const size_t section_no = section_table[tab_no];
					const auto& font = SimpleGUI::GetFont();
					if (auto section = resource_info->getSection(section_no))
					{
						double offset_y = 10 - tag_scroll[section_no - 1].y;
						const auto& tags = section->getTags();
						for (size_t i = 0; i < tags.size(); ++i)
						{
							font(tags[i]->getName()).draw(10, offset_y, Palette::Dimgray);
							if (select_tag_no[section_no - 1].has_value()
								&& i == select_tag_no[section_no - 1].value())
							{
								auto select_rect = font(tags[i]->getName())
									.region(10, offset_y).stretched(2, 0);
								select_rect.w = Max(select_rect.w, tag_render_rect.w - 20);
								sip::drawDotRect(select_rect);
							}
							offset_y += 35;
						}
					}
				}
				tag_render_rect.rounded(5.0)
					.drawShadow({ -3, -3 }, 5.0, 0.0, Palette::Darkgray)
					.drawShadow({  3,  3 }, 5.0, 0.0, Palette::Whitesmoke)
					.draw(col_mng->getMainBackground());
				tag_render_target.draw(tag_render_rect.pos);
			}

			// リソースの描画
			if (resource_info)
			{
				resource_render_target.clear(col_mng->getMainBackground());
				{
					ScopedRenderTarget2D target{ resource_render_target };
					const size_t tab_no     = tab->getActiveTabIndex();
					const size_t section_no = section_table[tab_no];
					const auto&  tag_no = select_tag_no[section_no - 1];
					const auto& font = SimpleGUI::GetFont();
					const auto& section = resource_info->getSection(section_no);
					if (tag_no && section)
					{
						double offset_y = 10 - resource_scroll[section_no - 1].y;
						const auto& tag = section->getTag(tag_no.value());
						const auto& resources = tag->getResources();
						for (size_t i = 0; i < resources.size(); ++i)
						{
							bool enable = resources[i]->isEnable();
							drawEnable(enable, Vec2{ 10, offset_y + 5 }, 30, Palette::Royalblue, Palette::Silver);
							font(resources[i]->getPath()).draw(65, offset_y, Palette::Dimgray);
							if (select_resource_no[section_no - 1].has_value()
								&& i == select_resource_no[section_no - 1].value())
							{
								auto select_rect = font(resources[i]->getPath())
									.region(65, offset_y).stretched(2, 0);
								select_rect.w = Max(select_rect.w, resource_render_rect.w - 20);
								sip::drawDotRect(select_rect);
							}
							offset_y += 35;
						}
					}
				}
				resource_render_rect.rounded(5.0)
					.drawShadow({ -3, -3 }, 5.0, 0.0, Palette::Darkgray)
					.drawShadow({  3,  3 }, 5.0, 0.0, Palette::Whitesmoke)
					.draw(col_mng->getMainBackground());
				resource_render_target.draw(resource_render_rect.pos);
			}

			// メニューバーの描画
			menubar.draw();

			// ダイアログの描画
			dlg_mng->draw();

		} while (false);
	}
}