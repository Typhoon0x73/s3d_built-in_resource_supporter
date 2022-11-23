#include "MoveResourceCommand.h"
#include "../stdafx.h"
#include "../ResourceInfo/ResourceInfo.h"
#include "../ResourceInfo/ResourceSection.h"
#include "../ResourceInfo/ResourceTag.h"

namespace sip
{
	MoveResourceCommand::MoveResourceCommand(const CreateInfo& info) noexcept
		: info_{ info }
	{
	}

	MoveResourceCommand::~MoveResourceCommand() noexcept
	{
	}

	bool MoveResourceCommand::execute() noexcept
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
		auto prev_tag = section->getTag(info_.prev_tag);
		if (prev_tag == nullptr)
		{
			Logger << U"移動前のタグが存在しません。\n";
			return false;
		}
		auto&& resource = prev_tag->moveResource(info_.prev_index);
		if (resource == nullptr)
		{
			Logger << U"リソースが存在しません。\n";
			return false;
		}
		auto next_tag = (
			info_.prev_tag.compare(info_.next_tag) == 0
			? prev_tag
			: section->getTag(info_.next_tag)
			);
		if (next_tag == nullptr)
		{
			Logger << U"移動先のタグが存在しません。\n";
			return false;
		}
		if (!next_tag->insert(std::move(resource), info_.next_index))
		{
			Logger << U"移動先のタグへの挿入が失敗しました。\n";
			return false;
		}
		return true;
	}

	bool MoveResourceCommand::undo() noexcept
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
		auto next_tag = section->getTag(info_.next_tag);
		if (next_tag == nullptr)
		{
			Logger << U"移動先のタグが存在しません。\n";
			return false;
		}
		auto&& resource = next_tag->moveResource(info_.next_index);
		if (resource == nullptr)
		{
			Logger << U"リソースが存在しません。\n";
			return false;
		}
		auto prev_tag = (
			info_.prev_tag.compare(info_.next_tag) == 0
			? next_tag
			: section->getTag(info_.prev_index)
			);
		if (prev_tag == nullptr)
		{
			Logger << U"移動前のタグが存在しません。\n";
			return false;
		}
		if (!prev_tag->insert(std::move(resource), info_.prev_index))
		{
			Logger << U"移動前のタグへの挿入が失敗しました。\n";
			return false;
		}
		return true;
	}

	bool MoveResourceCommand::redo() noexcept
	{
		return execute();
	}

	std::string MoveResourceCommand::log() const noexcept
	{
		return "SwapResourceCommand : \n"
			+ Unicode::Narrow(U"\t section    : {}\n"_fmt(info_.section))
			+ Unicode::Narrow(U"\t prev_tag   : {}\n"_fmt(info_.prev_tag))
			+ Unicode::Narrow(U"\t next_tag   : {}\n"_fmt(info_.next_tag))
			+ Unicode::Narrow(U"\t prev_index : {}\n"_fmt(info_.prev_index))
			+ Unicode::Narrow(U"\t next_index : {}\n"_fmt(info_.next_index))
			;
	}
}
