# include <Siv3D.hpp> // OpenSiv3D v0.6.6
# include "ApplicationData.h"
# include "SettingsINI.h"
# include "WindowInfo.h"
# include "Command/CommandManager.h"
# include "Command/ToggleEnableCommand.h"
# include "ThemeColor/ThemeColorManager.h"
# include "Dialog/DialogManager.h"
# include "MenuFunc.h"
# include "Tab/SimpleTab.h"
# include "ResourceInfo/ResourceInfo.h"
# include "ResourceInfo/ResourceSection.h"
# include "ResourceInfo/ResourceTag.h"
# include "ResourceInfo/ResourceData.h"
# include "ToolDefine.h"
# include "Part/TagView.h"

using namespace sip;

namespace sip
{
	/// @brief 
	const size_t section_table[2] = { 2, 1 };

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

	RoundRect getToggleEnableRect(const Vec2& pos, double height)
	{
		return RectF{ pos, height * 1.5, height }.rounded(height * 0.5);
	}

	RoundRect drawEnable(bool enable, const Vec2& pos, double height, const ColorF& active_color, const ColorF& disable_color)
	{
		Circle circle{ { 0.0, 0.0 }, height * 0.4 };
		auto rect = getToggleEnableRect(pos, height);
		ColorF color = (enable ? active_color : disable_color);
		rect.draw(color)
			.drawFrame(2.0, 0.0, ColorF{ 0.25, 0.25 }, ColorF{ 0.75, 0.75 })
			;
		circle.setPos(rect.center() + Vec2{ height * 0.25 * (enable ? -1 : 1), 0});
		circle.draw(Palette::Gainsboro);
		return rect;
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

	// データ
	PageListParam tag_page_list_params[2];
	Array<PageListParam> resource_page_list_params[2];

	// タブ
	Array<String> tab_items = { U"User", U"Engine" };
	TabPtr simple_tab(new SimpleTab(SimpleGUI::GetFont(), Vec2{ 100, 110 }, Size{ 100, 30 }, tab_items));
	size_t select_tab_no = 0;

	// タグ
	Optional<size_t> select_tag_no[] = { none, none };
	RectF tag_render_rect{
		simple_tab->getPos().x, simple_tab->getPos().y + simple_tab->getTabRect(0).h + 10,
		200, 400
	};
	TagView tag_view{
		tag_render_rect,
		&select_tab_no,
		{ &(tag_page_list_params[0]), &(tag_page_list_params[1]) }
	};

	// リソース
	Optional<size_t> select_resource_no[] = { none, none };
	bool is_only_file_name = false;
	RectF regist_button_rect{ 0, 0, 220, 30 };
	RectF resource_render_rect{
		tag_render_rect.rightX() + 10, tag_render_rect.y,
		400, tag_render_rect.h
	};
	RectF toggle_render_rect{
		resource_render_rect.stretched(0, -300, 0, -10)
	};
	Vec2 resource_scroll[] = { { 0.0, 0.0 }, { 0.0, 0.0 } };
	RenderTexture resource_render_target((uint32)resource_render_rect.w, (uint32)resource_render_rect.h);
	RenderTexture toggle_render_target((uint32)toggle_render_rect.w, (uint32)toggle_render_rect.h);
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
			for (size_t i = 0; i < menu_item_name_count; i++)
			{
				for (size_t k = 0; k < menu_item_table[i].items.size(); k++)
				{
					menubar.setItemEnabled(i, k, menu_item_table[i].enable_funcs[k]());
				}
			}
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
			if (simple_tab)
			{
				simple_tab->update();
				select_tab_no = simple_tab->getActiveTabIndex();
			}

			// タグの選択
			tag_view.update();

			// ファイル名のみ表示切り替え
			if (getToggleEnableRect(resource_render_rect.pos - Vec2{ -20, 35 }, 26).leftClicked())
			{
				is_only_file_name = !is_only_file_name;
			}

			// リソースの選択
			if (resource_info)
			{
				const auto tab_no     = simple_tab->getActiveTabIndex();
				const auto section_no = section_table[tab_no];
				const auto tag_no     = select_tag_no[section_no - 1];
				const auto& font = SimpleGUI::GetFont();
				const auto& section = resource_info->getSection(section_no);
				if (tab_no == 0 && MenuEnableFunc::isOpen())
				{
					auto line_y = resource_render_rect.h - (regist_button_rect.h + 20);
					auto button_rect = regist_button_rect
						.movedBy((resource_render_rect.w - regist_button_rect.w) * 0.5, line_y + 10.0)
						.movedBy(resource_render_rect.pos);
					if (button_rect.leftClicked())
					{
						if (!MenuFunc::registResource())
						{

						}
					}
				}
				if (tag_no && section)
				{
					double offset_y = 10 - resource_scroll[section_no - 1].y + resource_render_rect.y;
					if (const auto& tag = section->getTag(tag_no.value()))
					{
						const auto& resources = tag->getResources();
						for (size_t i = 0; i < resources.size(); ++i)
						{
							auto toggle_rect = getToggleEnableRect(Vec2{ 10, offset_y + 6 }, 24).movedBy(resource_render_rect.x, 0);
							if (toggle_rect.leftClicked())
							{
								ToggleEnableCommand::CreateInfo info;
								info.section = section_no;
								info.tag = tag->getName();
								info.index = i;
								if (!cmd_mng->regist(std::move(std::make_unique<ToggleEnableCommand>(info))))
								{

								}
							}
							FilePath path{ resources[i]->getPath() };
							if (is_only_file_name)
							{
								path = FileSystem::FileName(FileSystem::FullPath(path));
							}
							auto select_rect = font(path)
								.region(resource_render_rect.x + 65, offset_y);
							select_rect.w = resource_render_rect.w - 70;
							if (select_rect.leftClicked())
							{
								select_resource_no[section_no - 1] = i;
								break;
							}
							offset_y += 35;
						}
						resource_page_size[section_no - 1].y = resources.size() * 35 + 20 + regist_button_rect.h + 20;
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
			if (simple_tab)
			{
				simple_tab->draw();
			}

			// タグの描画
			tag_view.draw();

			// ファイル名のみ表示切り替えボタン描画
			auto toggle_file_name_rect = drawEnable(
				is_only_file_name, resource_render_rect.pos - Vec2{ -20, 35 }, 26,
				Palette::Royalblue, Palette::Silver
			);
			SimpleGUI::GetFont()(U"show file name only")
				.draw(toggle_file_name_rect.rect.tr().x + 5, toggle_file_name_rect.y - 5, Palette::Dimgray);

			// リソースの描画
			if (resource_info)
			{
				resource_render_target.clear(col_mng->getMainBackground());
				{
					ScopedRenderTarget2D target{ resource_render_target };
					const size_t tab_no     = simple_tab->getActiveTabIndex();
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
							drawEnable(enable, Vec2{ 10, offset_y + 6 }, 24, Palette::Royalblue, Palette::Silver);
							FilePath path{ resources[i]->getPath() };
							if (is_only_file_name)
							{
								path = FileSystem::FileName(FileSystem::FullPath(path));
							}
							font(path).draw(65, offset_y, Palette::Dimgray);
							if (select_resource_no[section_no - 1].has_value()
								&& i == select_resource_no[section_no - 1].value())
							{
								auto select_rect = font(path)
									.region(65, offset_y).stretched(2, 0);
								select_rect.w = Max(select_rect.w, resource_render_rect.w - 70);
								sip::drawDotRect(select_rect);
							}
							offset_y += 35;
						}
					}
					if (tab_no == 0 && MenuEnableFunc::isOpen())
					{
						auto line_y = resource_render_rect.h - (regist_button_rect.h + 20);
						RectF line_rect{ 0, line_y, resource_render_rect.size };
						line_rect
							.drawShadow({ 0, -2 }, 5.0, 0.0, Palette::Whitesmoke)
							.draw(col_mng->getMainBackground());
						auto button_color =
							(regist_button_rect
								.movedBy((resource_render_rect.w - regist_button_rect.w) * 0.5, line_y + 10.0)
								.movedBy(resource_render_rect.pos).mouseOver()
							? ColorF{ Palette::Gainsboro } 
							: col_mng->getMainBackground());
						auto draw_regist_region = regist_button_rect
							.movedBy((resource_render_rect.w - regist_button_rect.w) * 0.5, line_y + 10.0)
							.rounded(5.0)
							.drawShadow({  3,  3 }, 5.0, 0.0, Palette::Darkgray)
							.drawShadow({ -3, -3 }, 5.0, 0.0, Palette::Whitesmoke)
							.draw(button_color);
						SimpleGUI::GetFont()(U"regist resource").drawAt(draw_regist_region.center(), Palette::Dimgray);
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
