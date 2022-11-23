#include "../stdafx.h"
#include "RegistResourceCommand.h"
#include "../XMLInfo/EditFilters.h"
#include "../XMLInfo/EditVcxproj.h"
#include "../ResourceInfo/ResourceInfo.h"
#include "../ResourceInfo/ResourceSection.h"
#include "../ResourceInfo/ResourceTag.h"
#include "../ResourceInfo/ResourceFactory.h"

namespace sip
{
	RegistResourceCommand::RegistResourceCommand(const CreateInfo& info) noexcept
		: info_{ info }
		, created_guid_{ U"" }
		, has_regist_tag_{ false }
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
		if (edit_vcxproj == nullptr
			|| edit_filters == nullptr
			|| resource_info == nullptr
			|| relative_path == nullptr)
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
		auto edit_vcxproj = g_pGetBlackboard(EditVcxproj* const)->get("edit_vcxproj");
		auto edit_filters = g_pGetBlackboard(EditFilters* const)->get("edit_filters");
		auto resource_info = g_pGetBlackboard(ResourceInfo* const)->get("resource_info");
		auto relative_path = g_pGetBlackboard(FilePath* const)->get("relative_path");
		if (edit_vcxproj == nullptr
			|| edit_filters == nullptr
			|| resource_info == nullptr
			|| relative_path == nullptr)
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
		return execute();
	}

	std::string RegistResourceCommand::log() const noexcept
	{
		return "RegistResourceCommand : \n"
			+ Unicode::Narrow(U"\t section    : {}\n"_fmt(info_.section))
			+ Unicode::Narrow(U"\t tag        : {}\n"_fmt(info_.tag))
			+ Unicode::Narrow(U"\t path       : {}\n"_fmt(info_.path))
			+ Unicode::Narrow(U"\t filter     : {}\n"_fmt(info_.filter))
			+ Unicode::Narrow(U"\t extensions : {}\n"_fmt(info_.extensions))
			;
	}
}
