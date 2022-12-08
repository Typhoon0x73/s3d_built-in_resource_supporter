#include "../stdafx.h"
#include "RegistResourceCommand.h"
#include "../XMLInfo/EditFilters.h"
#include "../XMLInfo/EditVcxproj.h"
#include "../ResourceInfo/ResourceInfo.h"
#include "../ResourceInfo/ResourceSection.h"
#include "../ResourceInfo/ResourceTag.h"
#include "../ResourceInfo/ResourceFactory.h"
#include "../ToolDefine.h"

namespace sip
{
	RegistResourceCommand::RegistResourceCommand(const CreateInfo& info) noexcept
		: info_{ info }
		, created_guid_{ U"" }
		, has_regist_tag_{ false }
		, old_select_tag_{ std::nullopt }
		, old_select_res_{ std::nullopt }
	{
	}

	RegistResourceCommand::~RegistResourceCommand() noexcept
	{
	}

	bool RegistResourceCommand::execute() noexcept
	{
		auto edit_vcxproj = g_pGetBlackboard(EditVcxproj* const)->get("edit_vcxproj");
		auto edit_filters = g_pGetBlackboard(EditFilters* const)->get("edit_filters");
		auto resource_info = g_pGetBlackboard(ResourceInfo* const)->get("resource_info");
		auto relative_path = g_pGetBlackboard(FilePath* const)->get("relative_path");
		auto resource_page_list_params = g_pGetBlackboard(ResourceParams* const)->get("resource_page_list_params");
		if (edit_vcxproj == nullptr
			|| edit_filters == nullptr
			|| resource_info == nullptr
			|| relative_path == nullptr
			|| resource_page_list_params == nullptr)
		{
			Logger << U"アプリケーションデータの取得に失敗しました。\n";
			return false;
		}
		auto section = resource_info->getSection(info_.section);
		if (section == nullptr)
		{
			Logger << U"セクションが存在しません。\n";
			return false;
		}
		auto tag = section->getTag(info_.tag, has_regist_tag_);
		if (tag == nullptr)
		{
			if (!section->registTag(info_.tag))
			{
				Logger << U"タグの登録に失敗しました。\n";
				return false;
			}
			has_regist_tag_ = true;
			tag = section->getTag(info_.tag);
			if (tag == nullptr)
			{
				Logger << U"タグが存在しません。\n";
				return false;
			}
		}
		tag->regist(ResourceFactory::Create(info_.path, true));
		if (!edit_filters->existFilter(info_.filter))
		{
			String guid = created_guid_;
			if (guid.compare(U"") == 0)
			{
				do
				{
					auto uuid = UUIDValue::Generate();
					guid = U"{" + uuid.str() + U"}";
				}
				while (edit_filters->existGUID(guid) || edit_vcxproj->getProjectGUID().compare(guid) == 0);
				created_guid_ = guid;
			}
			if (!edit_filters->addFilter(info_.filter, guid, info_.extensions))
			{
				Logger << U"フィルターの追加に失敗しました。\n";
				return false;
			}
		}
		if (has_regist_tag_)
		{
			size_t select_tab_no;
			for (select_tab_no = 0; select_tab_no < 2; ++select_tab_no)
			{
				if (section_table[select_tab_no] == info_.section)
				{
					break;
				}
			}
			(*resource_page_list_params)[select_tab_no].push_back(PageListParam());
			if (!edit_filters->addItemGroup(info_.tag))
			{
				Logger << U"アイテムグループ(filters)の追加に失敗しました。\n";
				return false;
			}
			if (!edit_vcxproj->addItemGroup(info_.tag))
			{
				Logger << U"アイテムグループ(vcxproj)の追加に失敗しました。\n";
				return false;
			}
		}
		if (!edit_filters->addItem(info_.tag, (*relative_path) + info_.path, info_.filter))
		{
			Logger << U"アイテム(filters)の追加に失敗しました。\n";
			return false;
		}
		if (!edit_vcxproj->addItem(info_.tag, (*relative_path) + info_.path))
		{
			Logger << U"アイテム(vcxproj)の追加に失敗しました。\n";
			return false;
		}
		return true;
	}

	bool RegistResourceCommand::undo() noexcept
	{
		auto tag_params      = g_pGetBlackboard(TagParams* const)->get("tag_page_list_params");
		auto resource_params = g_pGetBlackboard(ResourceParams* const)->get("resource_page_list_params");
		auto tab_no          = g_pGetBlackboard(size_t* const)->get("select_tab_no");
		if (tag_params && resource_params && tab_no)
		{
			if (auto tag_no = (*tag_params)[*tab_no].select_no)
			{
				old_select_tag_ = tag_no.value();
				auto res_no = (*resource_params)[*tab_no][tag_no.value()].select_no;
				if (res_no.has_value())
				{
					old_select_res_ = res_no.value();
				}
				else
				{
					old_select_res_ = std::nullopt;
				}
			}
			else
			{
				old_select_tag_ = std::nullopt;
			}
		}
		auto edit_vcxproj = g_pGetBlackboard(EditVcxproj* const)->get("edit_vcxproj");
		auto edit_filters = g_pGetBlackboard(EditFilters* const)->get("edit_filters");
		auto resource_info = g_pGetBlackboard(ResourceInfo* const)->get("resource_info");
		auto relative_path = g_pGetBlackboard(FilePath* const)->get("relative_path");
		auto resource_page_list_params = g_pGetBlackboard(ResourceParams* const)->get("resource_page_list_params");
		auto tag_page_list_params = g_pGetBlackboard(TagParams* const)->get("tag_page_list_params");
		if (edit_vcxproj == nullptr
			|| edit_filters == nullptr
			|| resource_info == nullptr
			|| relative_path == nullptr
			|| tag_page_list_params == nullptr
			|| resource_page_list_params == nullptr)
		{
			Logger << U"アプリケーションデータの取得に失敗しました。\n";
			return false;
		}
		auto section = resource_info->getSection(info_.section);
		if (section == nullptr)
		{
			Logger << U"セクションが存在しません。\n";
			return false;
		}
		auto tag = section->getTag(info_.tag);
		if (tag == nullptr)
		{
			Logger << U"タグが存在しません。\n";
			return false;
		}
		if (!edit_vcxproj->eraseItem(info_.tag, (*relative_path) + info_.path))
		{
			Logger << U"アイテム(vcxproj)の削除に失敗しました。\n";
			return false;
		}
		if (!edit_filters->eraseItem(info_.tag, (*relative_path) + info_.path))
		{
			Logger << U"アイテム(filters)の削除に失敗しました。\n";
			return false;
		}
		if (created_guid_.compare(U"") != 0)
		{
			if (!edit_filters->eraseFilter(info_.filter))
			{
				Logger << U"フィルターの削除に失敗しました。\n";
				return false;
			}
		}
		tag->erase(info_.path);
		if (has_regist_tag_)
		{
			auto tag_no = (*tag_page_list_params)[*tab_no].select_no;
			(*resource_page_list_params)[*tab_no].erase((*resource_page_list_params)[*tab_no].begin() + tag_no.value());
			int64_t tag_max = (*resource_page_list_params)[*tab_no].size();
			if (tag_max <= tag_no.value())
			{
				if (tag_max - 1 >= 0)
				{
					(*tag_page_list_params)[*tab_no].select_no = tag_max - 1;
				}
			}
			if (!edit_vcxproj->eraseItemGroup(info_.tag))
			{
				Logger << U"アイテムグループ(vcxproj)の削除に失敗しました。\n";
				return false;
			}
			if (!edit_filters->eraseItemGroup(info_.tag))
			{
				Logger << U"アイテムグループ(filters)の削除に失敗しました。\n";
				return false;
			}
			if (!section->eraseTag(info_.tag))
			{
				Logger << U"タグの削除に失敗しました。\n";
				return false;
			}
		}
		return true;
	}

	bool RegistResourceCommand::redo() noexcept
	{
		auto result = RegistResourceCommand::execute();
		auto tag_params      = g_pGetBlackboard(TagParams* const)->get("tag_page_list_params");
		auto resource_params = g_pGetBlackboard(ResourceParams* const)->get("resource_page_list_params");
		auto tab_no          = g_pGetBlackboard(size_t* const)->get("select_tab_no");
		if (tag_params && resource_params && tab_no)
		{
			if (auto tag_no = old_select_tag_)
			{
				(*tag_params)[*tab_no].select_no = tag_no.value();
				if (auto res_no = old_select_res_)
				{
					(*resource_params)[*tab_no][tag_no.value()].select_no = res_no.value();
				}
				else
				{
					(*resource_params)[*tab_no][tag_no.value()].select_no = std::nullopt;
				}
			}
			else
			{
				(*tag_params)[*tab_no].select_no = std::nullopt;
			}
		}
		return result;
	}

	std::string RegistResourceCommand::log() const noexcept
	{
		return "RegistResourceCommand : \n"
			+ Unicode::Narrow(U"\t section    : {}\n"_fmt(info_.section))
			+ Unicode::Narrow(U"\t tag        : {}\n"_fmt(info_.tag))
			+ Unicode::Narrow(U"\t path       : {}\n"_fmt(info_.path))
			+ Unicode::Narrow(U"\t filter     : {}\n"_fmt(info_.filter))
			+ Unicode::Narrow(U"\t extensions : {}\n"_fmt(info_.extensions))
			+ Unicode::Narrow(U"\t select_tag : {}\n"_fmt(old_select_tag_.has_value()))
			+ Unicode::Narrow(U"\t select_res : {}\n"_fmt(old_select_res_.has_value()))
			;
	}
}
