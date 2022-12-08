#include "stdafx.h"
#include "ApplicationData.h"
#include "WindowInfo.h"
#include "Command/CommandManager.h"
#include "Dialog/DialogManager.h"
#include "ThemeColor/ThemeColorManager.h"
#include "ResourceInfo/ResourceInfo.h"
#include "XMLInfo/XMLInfo.h"
#include "XMLInfo/EditVcxproj.h"
#include "XMLInfo/EditFilters.h"
#include "SettingsINI.h"

namespace sip
{
	ApplicationData::ApplicationData() noexcept
		: window_info_{ new WindowInfo{ Size{ 800, 600 }, SizeF{ 1.0, 1.0 } } }
		, command_manager_{ new CommandManager() }
		, theme_color_manager_{ new ThemeColorManager() }
		, dialog_manager_{ new DialogManager() }
		, vcxproj_info_{ new XMLInfo() }
		, filters_info_{ new XMLInfo() }
		, edit_vcxproj_{ new EditVcxproj() }
		, edit_filters_{ new EditFilters() }
		, resource_info_{ new ResourceInfo() }
		, relative_path_{ new FilePath() }
		, vcxproj_path_{ new FilePath() }
		, resource_path_{ new FilePath() }
		, tag_page_list_params_{ new TagParams() }
		, resource_page_list_params_{ new ResourceParams() }
		, is_only_file_name_{ new bool(false) }
		, select_tab_no_{ new size_t(0) }
		, gui_font_{ new Font(FontMethod::MSDF, 20, Typeface::CJK_Regular_JP) }
	{
		// ウィンドウ情報を登録
		g_pGetBlackboard(WindowInfo* const)->insert("window_info", window_info_.get());

		// 各マネージャーの登録
		theme_color_manager_->SetTheme(ThemeColor::Light);
		g_pGetBlackboard(CommandManager*    const)->insert("command_manager"    , command_manager_.get());
		g_pGetBlackboard(ThemeColorManager* const)->insert("theme_color_manager", theme_color_manager_.get());
		g_pGetBlackboard(DialogManager*     const)->insert("dialog_manager"     , dialog_manager_.get());

		// XML/Resourceデータの登録
		g_pGetBlackboard(XMLInfo*         const)->insert("vcxproj_info" , vcxproj_info_.get());
		g_pGetBlackboard(XMLInfo*         const)->insert("filters_info" , filters_info_.get());
		g_pGetBlackboard(EditVcxproj*     const)->insert("edit_vcxproj" , edit_vcxproj_.get());
		g_pGetBlackboard(EditFilters*     const)->insert("edit_filters" , edit_filters_.get());
		g_pGetBlackboard(ResourceInfo*    const)->insert("resource_info", resource_info_.get());
		g_pGetBlackboard(FilePath*        const)->insert("relative_path", relative_path_.get());
		g_pGetBlackboard(FilePath*        const)->insert("vcxproj_path" , vcxproj_path_.get());
		g_pGetBlackboard(FilePath*        const)->insert("resource_path", resource_path_.get());
		g_pGetBlackboard(TagParams*       const)->insert("tag_page_list_params"     , tag_page_list_params_.get());
		g_pGetBlackboard(ResourceParams*  const)->insert("resource_page_list_params", resource_page_list_params_.get());
		g_pGetBlackboard(bool*            const)->insert("is_only_file_name", is_only_file_name_.get());
		g_pGetBlackboard(size_t*          const)->insert("select_tab_no", select_tab_no_.get());
		g_pGetBlackboard(Font*            const)->insert("gui_font"     , gui_font_.get());
		g_pGetBlackboard(ApplicationData* const)->insert("app_data"     , this);

		init();
	}

	ApplicationData::~ApplicationData() noexcept
	{
		// 解放
		FileSystem::ChangeCurrentDirectory(FileSystem::InitialDirectory());
		//SASSERT(SaveSettingsINI(String(title_name) + U".ini"));
		g_BlackboardRelease(ApplicationData* const);
		g_BlackboardRelease(Font* const);
		g_BlackboardRelease(size_t* const);
		g_BlackboardRelease(bool* const);
		g_BlackboardRelease(ResourceParams* const);
		g_BlackboardRelease(TagParams* const);
		g_BlackboardRelease(FilePath* const);
		g_BlackboardRelease(ResourceInfo* const);
		g_BlackboardRelease(EditFilters* const);
		g_BlackboardRelease(EditVcxproj* const);
		g_BlackboardRelease(XMLInfo* const);
		g_BlackboardRelease(const Font* const);
		g_BlackboardRelease(DialogManager* const);
		g_BlackboardRelease(ThemeColorManager* const);
		g_BlackboardRelease(CommandManager* const);
		g_BlackboardRelease(WindowInfo* const);
	}

	void ApplicationData::init() noexcept
	{
		// 初期化
		*resource_path_ = U"";
		*vcxproj_path_  = U"";
		resource_info_->clear();
		vcxproj_info_->clear();
		filters_info_->clear();
		command_manager_->clear();
		tag_page_list_params_->clear();
		resource_page_list_params_->clear();
		for (size_t i = 0; i < 2; i++)
		{
			tag_page_list_params_->emplace_back(PageListParam());
			resource_page_list_params_->emplace_back(TagParams());
		}
	}
}

