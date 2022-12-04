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
		, base_rect_{ Arg::center_<RectF::position_type>(Scene::CenterF()), Scene::Size().x * 0.7, Scene::Size().y * 0.6 }
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
		base_rect_.rounded(10)
			.drawShadow({ -5, -5 }, 10.0, 3.0, Palette::Whitesmoke)
			.drawShadow({  5,  5 }, 10.0, 3.0, Palette::Darkgray)
			.draw(col_mng->getMainBackground());

		RectF path_rect{ base_rect_.x, base_rect_.y, base_rect_.w * 0.7, 30 };
		path_rect.movedBy(40, 40).rounded(5)
			.drawShadow({  1,  1 }, 3.0, 0.0, Palette::Whitesmoke)
			.drawShadow({ -1, -1 }, 3.0, 0.0, Palette::Darkgray)
			.draw(col_mng->getMainBackground());

		RectF load_rect{ path_rect.rightX() + 30, base_rect_.y, 30, 30 };
		load_rect.movedBy(40, 40).rounded(5)
			.drawShadow({ -1, -1 }, 5.0, 0.0, Palette::Whitesmoke)
			.drawShadow({  1,  1 }, 5.0, 0.0, Palette::Darkgray)
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
