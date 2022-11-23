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
#include "../stdafx.h"

namespace sip
{
	RegistResourceDialog::RegistResourceDialog() noexcept
		: result_{ Result::None }
		, edit_{}
		, tag_list_box_state_{}
		, filter_list_box_state_{}
		, load_resource_button_region_{ SimpleGUI::ButtonRegion(U"load", { 230, 10 }) }
		, add_resource_button_region_{ SimpleGUI::ButtonRegion(U"add", { 10, 50 }) }
		, cancel_resource_button_region_{ SimpleGUI::ButtonRegion(U"cancel", { 100, 50 }) }
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
		if (load_resource_button_region_.leftClicked())
		{
			load_resource();
		}
		if (edit_.path.length() && add_resource_button_region_.leftClicked())
		{
			RegistResourceCommand::CreateInfo info;
			info.section    = 2;
			info.tag        = edit_.tag;
			info.path       = edit_.path;
			info.filter     = edit_.filter;
			info.extensions = edit_.extensions;
			if (!command_manager->regist(std::make_unique<RegistResourceCommand>(info)))
			{
				Logger << U"リソース登録失敗。\n";
				result_ = Result::Failed;
			}
			result_ = Result::Success;
		}
		if (cancel_resource_button_region_.leftClicked())
		{
			result_ = Result::Cancel;
		}
		if (tag_list_box_state_.selectedItemIndex)
		{
			if (tag_list_box_state_.items.size() > *tag_list_box_state_.selectedItemIndex)
			{
				edit_.tag = tag_list_box_state_.items.at(*tag_list_box_state_.selectedItemIndex);
			}
		}
		if (edit_.path.compare(U"") != 0)
		{
			tag_list_box_state_.items.clear();
			tag_list_box_state_.items.emplace_back(FileSystem::Extension(edit_.path).uppercased());
			for(const auto& tag : resource_info->getSection(2)->getTags())
			{
				if (tag->getName().compare(tag_list_box_state_.items[0]) == 0)
				{
					continue;
				}
				tag_list_box_state_.items.emplace_back(tag->getName());
			}
		}
		if (filter_list_box_state_.selectedItemIndex)
		{
			if (filter_list_box_state_.items.size() > *filter_list_box_state_.selectedItemIndex)
			{
				edit_.filter = filter_list_box_state_.items.at(*filter_list_box_state_.selectedItemIndex);
			}
		}
		if (edit_.filter.compare(U"") != 0)
		{
			filter_list_box_state_.items.clear();
			for (const auto& filter : edit_filters->getFilters()->getChildren())
			{
				auto elem = filter.second->findElement(U"Filter");
				if (elem == nullptr)
				{
					continue;
				}
				auto attrib = elem->findAttribute(U"Include");
				if (attrib == nullptr)
				{
					continue;
				}
				if (edit_.filter.compare(attrib->getValue()) == 0)
				{
					filter_list_box_state_.selectedItemIndex = filter_list_box_state_.items.size();
				}
				filter_list_box_state_.items.emplace_back(attrib->getValue());
			}
		}
		if (result_ != Result::None)
		{
			return false;
		}
		return true;
	}

	void RegistResourceDialog::draw() /* const */ noexcept
	{
		TextEditState te_state;
		te_state.text = edit_.path;
		SimpleGUI::TextBox(te_state, { 10, 10 }, 200, unspecified, false);
		SimpleGUI::Button(U"load", load_resource_button_region_.pos);
		SimpleGUI::Button(U"add", add_resource_button_region_.pos, unspecified, edit_.path.length());
		SimpleGUI::Button(U"cancel", cancel_resource_button_region_.pos);
		SimpleGUI::Headline(U"tag"   , {  10, 100 });
		SimpleGUI::Headline(U"filter", { 200, 100 });
		SimpleGUI::ListBox(tag_list_box_state_   , {  10, 140 });
		SimpleGUI::ListBox(filter_list_box_state_, { 200, 140 }, 500);
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
			tag_list_box_state_.selectedItemIndex = 0;
		}
	}
}
