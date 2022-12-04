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
#include "../stdafx.h"

namespace sip
{
	RegistResourceDialog::RegistResourceDialog() noexcept
		: result_{ Result::None }
		, edit_{}
	{
	}

	RegistResourceDialog::~RegistResourceDialog() noexcept
	{
	}

	bool RegistResourceDialog::update() noexcept
	{
		auto command_manager = g_pGetBlackboard(CommandManager* const)->get("command_manager");
		auto resource_info   = g_pGetBlackboard(ResourceInfo*   const)->get("resource_info");
		auto edit_filters    = g_pGetBlackboard(EditFilters*    const)->get("edit_filters");
		if (command_manager == nullptr)
		{
			Logger << U"マネージャーが正常に取得できませんでした。\n";
			result_ = Result::Failed;
			return false;
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
		RoundRect rect{ 0, 0, Scene::Width() * 0.7, Scene::Height() * 0.6, 10 };
		rect.setCenter(Scene::CenterF());
		rect.drawShadow({ -5, -5 }, 10.0, 3.0, Palette::Whitesmoke)
			.drawShadow({  5,  5 }, 10.0, 3.0, Palette::Darkgray)
			.draw(col_mng->getMainBackground());
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
		if (edit_.tag.compare(U"") == 0)
		{
			edit_.tag = FileSystem::Extension(edit_.path).uppercased();
			//tag_list_box_state_.selectedItemIndex = 0;
		}
	}
}
