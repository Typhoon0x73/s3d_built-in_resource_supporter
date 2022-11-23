#include "RegistTagCommand.h"
#include "../stdafx.h"
#include "../XMLInfo/EditFilters.h"
#include "../XMLInfo/EditVcxproj.h"
#include "../ResourceInfo/ResourceInfo.h"
#include "../ResourceInfo/ResourceSection.h"
#include "../ResourceInfo/ResourceTag.h"

namespace sip
{
	RegistTagCommand::RegistTagCommand(const CreateInfo& info) noexcept
		: info_{ info }
	{
	}

	RegistTagCommand::~RegistTagCommand() noexcept
	{
	}

	bool RegistTagCommand::execute() noexcept
	{
		auto edit_vcxproj = g_pGetBlackboard(EditVcxproj* const)->get("edit_vcxproj");
		auto edit_filters = g_pGetBlackboard(EditFilters* const)->get("edit_filters");
		auto resource_info = g_pGetBlackboard(ResourceInfo* const)->get("resource_info");
		if (edit_vcxproj == nullptr
			|| edit_filters == nullptr
			|| resource_info == nullptr)
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
		if (section->getTag(info_.tag))
		{
			Logger << U"タグが存在します。\n";
			return false;
		}
		if (!section->registTag(info_.tag))
		{
			Logger << U"タグの登録に失敗しました。\n";
			return false;
		}
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
		return true;
	}

	bool RegistTagCommand::undo() noexcept
	{
		auto edit_vcxproj = g_pGetBlackboard(EditVcxproj* const)->get("edit_vcxproj");
		auto edit_filters = g_pGetBlackboard(EditFilters* const)->get("edit_filters");
		auto resource_info = g_pGetBlackboard(ResourceInfo* const)->get("resource_info");
		if (edit_vcxproj == nullptr
			|| edit_filters == nullptr
			|| resource_info == nullptr)
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
		if (!section->eraseTag(info_.tag))
		{
			Logger << U"タグの削除に失敗しました。\n";
			return false;
		}
		if (!edit_filters->eraseItemGroup(info_.tag))
		{
			Logger << U"アイテムグループ(filters)の削除に失敗しました。\n";
			return false;
		}
		if (!edit_vcxproj->eraseItemGroup(info_.tag))
		{
			Logger << U"アイテムグループ(vcxproj)の削除に失敗しました。\n";
			return false;
		}
		return true;
	}

	bool RegistTagCommand::redo() noexcept
	{
		return execute();
	}

	std::string RegistTagCommand::log() const noexcept
	{
		return "RegistResourceCommand : \n"
			+ Unicode::Narrow(U"\t section : {}\n"_fmt(info_.section))
			+ Unicode::Narrow(U"\t tag     : {}\n"_fmt(info_.tag))
			;
	}
}
