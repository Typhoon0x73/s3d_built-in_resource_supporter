#include "EraseTagCommand.h"
#include "../stdafx.h"
#include "../XMLInfo/EditVcxproj.h"
#include "../XMLInfo/EditFilters.h"
#include "../ResourceInfo/ResourceInfo.h"
#include "../ResourceInfo/ResourceSection.h"
#include "../ResourceInfo/ResourceTag.h"

namespace sip
{
	EraseTagCommand::EraseTagCommand(const CreateInfo& info) noexcept
		: info_{ info }
		, rc_erase_index_{ 0 }
		, rc_evacuation_{ nullptr }
		, vcxproj_erase_index_{ 0 }
		, vcxproj_parent_erase_index_{ 0 }
		, vcxproj_evacuation_{ nullptr }
		, filters_erase_index_{ 0 }
		, filters_parent_erase_index_{ 0 }
		, filters_evacuation_{ nullptr }
	{
	}

	EraseTagCommand::~EraseTagCommand() noexcept
	{
	}

	bool EraseTagCommand::execute() noexcept
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
		if (!section->eraseTag(info_.tag, &rc_evacuation_, &rc_erase_index_))
		{
			Logger << U"タグの削除に失敗しました。\n";
			return false;
		}
		if (!edit_filters->eraseItemGroup(info_.tag, &filters_evacuation_, &filters_parent_erase_index_, &filters_erase_index_))
		{
			Logger << U"アイテムグループ(filters)の削除に失敗しました。\n";
		}
		if (!edit_vcxproj->eraseItemGroup(info_.tag, &vcxproj_evacuation_, &vcxproj_parent_erase_index_, &vcxproj_erase_index_))
		{
			Logger << U"アイテムグループ(vcxproj)の削除に失敗しました。\n";
		}
		return true;
	}

	bool EraseTagCommand::undo() noexcept
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
		if (!section->insertTag(std::move(rc_evacuation_), rc_erase_index_))
		{
			Logger << U"タグの挿入に失敗しました。\n";
			return false;
		}
		if (!edit_filters->insertItemGroup(info_.tag, filters_evacuation_, filters_parent_erase_index_, filters_erase_index_))
		{
			Logger << U"アイテムグループ(filters)の挿入に失敗しました。\n";
		}
		if (!edit_vcxproj->insertItemGroup(info_.tag, vcxproj_evacuation_, vcxproj_parent_erase_index_, vcxproj_erase_index_))
		{
			Logger << U"アイテムグループ(vcxproj)の挿入に失敗しました。\n";
		}
		return true;
	}

	bool EraseTagCommand::redo() noexcept
	{
		return execute();
	}

	std::string EraseTagCommand::log() const noexcept
	{
		return "EraseTagCommand : \n"
			+ Unicode::Narrow(U"\t section : {}\n"_fmt(info_.section))
			+ Unicode::Narrow(U"\t tag     : {}\n"_fmt(info_.tag))
			;
	}
}
