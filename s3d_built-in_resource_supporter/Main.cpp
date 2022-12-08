# include <Siv3D.hpp> // OpenSiv3D v0.6.6
# include "ApplicationData.h"
# include "SettingsINI.h"
# include "WindowInfo.h"
# include "Command/CommandManager.h"
# include "Command/ToggleEnableCommand.h"
# include "ThemeColor/ThemeColorManager.h"
# include "Dialog/DialogManager.h"
# include "Dialog/RegistResourceDialog.h"
# include "MenuFunc.h"
# include "Tab/SimpleTab.h"
# include "ResourceInfo/ResourceInfo.h"
# include "ResourceInfo/ResourceSection.h"
# include "ResourceInfo/ResourceTag.h"
# include "ResourceInfo/ResourceData.h"
# include "ToolDefine.h"
# include "Part/TagView.h"
# include "Part/ResourceView.h"

using namespace sip;

namespace sip
{
	/// @brief 
	constexpr size_t section_table[2] = { 2, 1 };

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

	Window::SetTitle(title_name);
	
	// 設定情報読み込み
	if (!LoadSettingsINI(String(title_name) + U".ini"))
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

	//dlg_mng->regist(std::move(std::make_unique<RegistResourceDialog>()));

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
	TabPtr simple_tab(new SimpleTab(SimpleGUI::GetFont(), Vec2{ 100, 110 }, Size{ 100, 30 }, tab_items));
	size_t* select_tab_no = g_pGetBlackboard(size_t* const)->get("select_tab_no");

	// タグ
	RectF tag_render_rect{
		simple_tab->getPos().x, simple_tab->getPos().y + simple_tab->getTabRect(0).h + 10,
		200, 400
	};
	TagView tag_view{ tag_render_rect };

	// リソース
	bool* is_only_file_name = g_pGetBlackboard(bool* const)->get("is_only_file_name");
	RectF resource_render_rect{
		tag_render_rect.rightX() + 10, tag_render_rect.y,
		400, tag_render_rect.h
	};
	ResourceView resource_view{ resource_render_rect };

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
					Logger << U"-- failed log --";
					for (const auto& cmd : cmd_mng->failedList())
					{
						Logger << Unicode::Widen(cmd->log());
					}
				}
			}
			// Ctrl + Y でやり直しコマンド実行
			if (KeyControl.pressed() && KeyY.down())
			{
				if (!MenuFunc::redo())
				{
					Logger << U"-- failed log --";
					for (const auto& cmd : cmd_mng->failedList())
					{
						Logger << Unicode::Widen(cmd->log());
					}
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
				*select_tab_no = simple_tab->getActiveTabIndex();
			}

			// タグの選択
			tag_view.update();

			// ファイル名のみ表示切り替え
			if (getToggleEnableRect(resource_render_rect.pos - Vec2{ -20, 35 }, 26).leftClicked())
			{
				*is_only_file_name = !(*is_only_file_name);
			}

			// リソースの選択
			resource_view.update();

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
				*is_only_file_name, resource_render_rect.pos - Vec2{ -20, 35 }, 26,
				Palette::Royalblue, Palette::Silver
			);
			SimpleGUI::GetFont()(U"show file name only")
				.draw(toggle_file_name_rect.rect.tr().x + 5, toggle_file_name_rect.y - 5, Palette::Dimgray);

			// リソースの描画
			resource_view.draw();

			// メニューバーの描画
			menubar.draw();
			
			// ダイアログの描画
			dlg_mng->draw();

		} while (false);
	}
}
