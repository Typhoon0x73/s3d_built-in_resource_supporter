#include "MoveTagCommand.h"
#include "../stdafx.h"
#include "../ResourceInfo/ResourceInfo.h"
#include "../ResourceInfo/ResourceSection.h"
#include "../ResourceInfo/ResourceTag.h"

namespace sip
{
	MoveTagCommand::MoveTagCommand(const CreateInfo& info) noexcept
		: info_{ info }
	{
	}

	MoveTagCommand::~MoveTagCommand() noexcept
	{
	}

	bool MoveTagCommand::execute() noexcept
	{
		auto resource_info = g_pGetBlackboard(ResourceInfo* const)->get("resource_info");
		if (resource_info == nullptr)
		{
			Logger << U"リソース情報の取得に失敗しました。\n";
			return false;
		}
		auto section = resource_info->getSection(info_.section);
		if (section == nullptr)
		{
			Logger << U"セクションが存在しません。\n";
			return false;
		}
		ResourceTagPtr evacute_tag;
		if (!section->eraseTag(info_.tag, &evacute_tag, &info_.prev_point))
		{
			Logger << U"タグの削除に失敗しました。\n";
			return false;
		}
		if (!section->insertTag(std::move(evacute_tag), info_.next_point))
		{
			Logger << U"タグの挿入に失敗しました。\n";
			return false;
		}
		return true;
	}

	bool MoveTagCommand::undo() noexcept
	{
		auto resource_info = g_pGetBlackboard(ResourceInfo* const)->get("resource_info");
		if (resource_info == nullptr)
		{
			Logger << U"リソース情報の取得に失敗しました。\n";
			return false;
		}
		auto section = resource_info->getSection(info_.section);
		if (section == nullptr)
		{
			Logger << U"セクションが存在しません。\n";
			return false;
		}
		ResourceTagPtr evacute_tag;
		if (!section->eraseTag(info_.tag, &evacute_tag, &info_.next_point))
		{
			Logger << U"タグの削除に失敗しました。\n";
			return false;
		}
		if (!section->insertTag(std::move(evacute_tag), info_.prev_point))
		{
			Logger << U"タグの挿入に失敗しました。\n";
			return false;
		}
		return true;
	}

	bool MoveTagCommand::redo() noexcept
	{
		return execute();
	}

	std::string MoveTagCommand::log() const noexcept
	{
		return "MoveTagCommand : \n"
			+ Unicode::Narrow(U"\t section    : {}\n"_fmt(info_.section))
			+ Unicode::Narrow(U"\t tag        : {}\n"_fmt(info_.tag))
			+ Unicode::Narrow(U"\t prev_point : {}\n"_fmt(info_.prev_point))
			+ Unicode::Narrow(U"\t next_point : {}\n"_fmt(info_.next_point))
			;
	}
}
