#include "stdafx.h"
#include "MenuFunc.h"
#include "ToolDefine.h"
#include "ApplicationData.h"
#include "ResourceInfo/ResourceInfo.h"
#include "ResourceInfo/ResourceSection.h"
#include "XMLInfo/XMLInfo.h"
#include "XMLInfo/XMLData.h"
#include "XMLInfo/EditVcxproj.h"
#include "XMLInfo/EditFilters.h"
#include "Command/CommandManager.h"
#include "Command/RegistResourceCommand.h"
#include "Dialog/DialogManager.h"
#include "Dialog/RegistResourceDialog.h"
#include "Dialog/EraseResourceDialog.h"
#include "Dialog/ExitApplicaitonDialog.h"

namespace sip
{
	bool MenuFunc::fileOpen() noexcept
	{
		auto open_path = Dialog::OpenFile({ { U"プロジェクトファイル", { U"vcxproj" } } });
		if (!open_path.has_value())
		{
			Logger << U"ファイルを開けませんでした。\n";
			return false;
		}
		auto resource_info = g_pGetBlackboard(ResourceInfo* const)->get("resource_info");
		auto vcxproj_info  = g_pGetBlackboard(XMLInfo*      const)->get("vcxproj_info");
		auto filters_info  = g_pGetBlackboard(XMLInfo*      const)->get("filters_info");
		auto edit_vcxproj  = g_pGetBlackboard(EditVcxproj*  const)->get("edit_vcxproj");
		auto edit_filters  = g_pGetBlackboard(EditFilters*  const)->get("edit_filters");
		auto relative_path = g_pGetBlackboard(FilePath*     const)->get("relative_path");
		auto vcxproj_path  = g_pGetBlackboard(FilePath*     const)->get("vcxproj_path");
		auto resource_path = g_pGetBlackboard(FilePath*     const)->get("resource_path");
		auto tag_page      = g_pGetBlackboard(TagParams*    const)->get("tag_page_list_params");
		auto resource_page = g_pGetBlackboard(ResourceParams* const)->get("resource_page_list_params");
		if (
			resource_info == nullptr
			|| vcxproj_info == nullptr
			|| filters_info == nullptr
			|| edit_vcxproj == nullptr
			|| edit_filters == nullptr
			|| relative_path == nullptr
			|| vcxproj_path == nullptr
			|| resource_path == nullptr
			|| tag_page == nullptr
			|| resource_page == nullptr
			)
		{
			Logger << U"アプリケーションデータが正常に取得できませんでした。\n";
			return false;
		}
		if (!MenuFunc::fileClose())
		{
			return false;
		}
		*vcxproj_path = FileSystem::FullPath(open_path.value());
		vcxproj_info->set(XMLReader(FilePathView(*vcxproj_path)));
		filters_info->set(XMLReader(FilePathView(*vcxproj_path + U".filters")));
		edit_vcxproj->set(*vcxproj_info);
		edit_filters->set(*filters_info);
		FileSystem::ChangeCurrentDirectory(FileSystem::ParentPath(*vcxproj_path));
		*resource_path = FileSystem::FullPath(edit_vcxproj->getResourcePath());
		*relative_path = FileSystem::RelativePath(FileSystem::ParentPath(*resource_path)).replaced(U'/', U'\\');
		FileSystem::ChangeCurrentDirectory(FileSystem::ParentPath(*resource_path));
		if (!resource_info->open(*resource_path))
		{
			return false;
		}
		for (size_t k : step(2))
		{
			auto section = resource_info->getSection(section_table[k]);
			if (section == nullptr)
			{
				continue;
			}
			(*tag_page)[k].init();
			(*resource_page)[k].clear();
			for (size_t i : step(section->getTagSize()))
			{
				_CRT_UNUSED(i);
				(*resource_page)[k].emplace_back(PageListParam());
			}
		}
		return true;
	}

	bool MenuFunc::fileSave() noexcept
	{
		auto resource_info = g_pGetBlackboard(ResourceInfo* const)->get("resource_info");
		auto vcxproj_info  = g_pGetBlackboard(XMLInfo*      const)->get("vcxproj_info");
		auto filters_info  = g_pGetBlackboard(XMLInfo*      const)->get("filters_info");
		auto edit_vcxproj  = g_pGetBlackboard(EditVcxproj*  const)->get("edit_vcxproj");
		auto edit_filters  = g_pGetBlackboard(EditFilters*  const)->get("edit_filters");
		auto relative_path = g_pGetBlackboard(FilePath*     const)->get("relative_path");
		auto vcxproj_path  = g_pGetBlackboard(FilePath*     const)->get("vcxproj_path");
		auto resource_path = g_pGetBlackboard(FilePath*     const)->get("resource_path");
		if (resource_info == nullptr
			|| vcxproj_info == nullptr
			|| filters_info == nullptr
			|| edit_vcxproj == nullptr
			|| edit_filters == nullptr
			|| relative_path == nullptr
			|| vcxproj_path == nullptr)
		{
			Logger << U"アプリケーションデータが正常に取得できませんでした。\n";
			return false;
		}
		if (!vcxproj_info->save(*vcxproj_path))
		{
			return false;
		}
		if (!filters_info->save(*vcxproj_path + U".filters"))
		{
			return false;
		}
		if (!resource_info->save(*resource_path))
		{
			return false;
		}
		return true;
	}

	bool MenuFunc::fileClose() noexcept
	{
		auto app_data = g_pGetBlackboard(ApplicationData* const)->get("app_data");
		if (app_data == nullptr)
		{
			Logger << U"アプリケーションデータが正常に取得できませんでした。\n";
			return false;
		}
		app_data->init();
		return true;
	}

	bool MenuFunc::undo() noexcept
	{
		auto command_manager = g_pGetBlackboard(CommandManager* const)->get("command_manager");
		if (command_manager == nullptr)
		{
			Logger << U"コマンドマネージャーが正常に取得できませんでした。\n";
			return false;
		}
		return command_manager->undo();
	}

	bool MenuFunc::redo() noexcept
	{
		auto command_manager = g_pGetBlackboard(CommandManager* const)->get("command_manager");
		if (command_manager == nullptr)
		{
			Logger << U"コマンドマネージャーが正常に取得できませんでした。\n";
			return false;
		}
		return command_manager->redo();
	}

	bool MenuFunc::registResource() noexcept
	{
		auto dialog_manager = g_pGetBlackboard(DialogManager* const)->get("dialog_manager");
		if (dialog_manager == nullptr)
		{
			Logger << U"マネージャーが正常に取得できませんでした。\n";
			return false;
		}
		dialog_manager->regist(std::move(std::make_unique<RegistResourceDialog>()));
		return true;
	}

	bool MenuFunc::eraseResource() noexcept
	{
		auto dialog_manager = g_pGetBlackboard(DialogManager* const)->get("dialog_manager");
		if (dialog_manager == nullptr)
		{
			Logger << U"マネージャーが正常に取得できませんでした。\n";
			return false;
		}
		dialog_manager->regist(std::move(std::make_unique<EraseResourceDialog>()));
		return true;
	}

	bool MenuFunc::appEnd() noexcept
	{
		if (MenuEnableFunc::existExecCommand()
			|| MenuEnableFunc::existUndoCommand())
		{
			auto dialog_manager = g_pGetBlackboard(DialogManager* const)->get("dialog_manager");
			if (dialog_manager == nullptr)
			{
				Logger << U"マネージャーが正常に取得できませんでした。\n";
				return false;
			}
			dialog_manager->regist(std::move(std::make_unique<ExitApplicationDialog>()));
		}
		else
		{
			System::Exit();
		}
		return true;
	}

	bool MenuEnableFunc::isOpen() noexcept
	{
		auto resource_info = g_pGetBlackboard(ResourceInfo* const)->get("resource_info");
		auto vcxproj_info  = g_pGetBlackboard(XMLInfo*      const)->get("vcxproj_info");
		auto filters_info  = g_pGetBlackboard(XMLInfo*      const)->get("filters_info");
		if(resource_info == nullptr
			|| vcxproj_info == nullptr
			|| filters_info == nullptr)
		{
			Logger << U"アプリケーションデータが正常に取得できませんでした。\n";
			return false;
		}
		return (resource_info->getSectionSize() > 0)
			&& (vcxproj_info->getRoot()->existData())
			&& (filters_info->getRoot()->existData())
			;
	}

	bool MenuEnableFunc::existExecCommand() noexcept
	{
		auto command_manager = g_pGetBlackboard(CommandManager* const)->get("command_manager");
		if (command_manager == nullptr)
		{
			Logger << U"コマンドマネージャーが正常に取得できませんでした。\n";
			return false;
		}
		return (command_manager->executeList().size() > 0);
	}

	bool MenuEnableFunc::existUndoCommand() noexcept
	{
		auto command_manager = g_pGetBlackboard(CommandManager* const)->get("command_manager");
		if (command_manager == nullptr)
		{
			Logger << U"コマンドマネージャーが正常に取得できませんでした。\n";
			return false;
		}
		return (command_manager->undoList().size() > 0);
	}

	bool MenuEnableFunc::existSelectUserResource() noexcept
	{
		auto tab_no               = g_pGetBlackboard(size_t* const)->get("select_tab_no");
		auto tag_page_params      = g_pGetBlackboard(TagParams* const)->get("tag_page_list_params");
		auto resource_page_params = g_pGetBlackboard(ResourceParams* const)->get("resource_page_list_params");
		if (tab_no == nullptr
			|| tag_page_params == nullptr
			|| resource_page_params == nullptr
			)
		{
			return false;
		}
		const auto tag_no = (*tag_page_params)[*tab_no].select_no;
		if (not tag_no.has_value())
		{
			return false;
		}
		const auto& page_param = (*resource_page_params)[*tab_no][tag_no.value()];
		return page_param.select_no.has_value();
	}
}
