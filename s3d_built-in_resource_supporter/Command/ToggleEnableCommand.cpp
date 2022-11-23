#include "ToggleEnableCommand.h"
#include "../stdafx.h"
#include "../ResourceInfo/ResourceInfo.h"
#include "../ResourceInfo/ResourceSection.h"
#include "../ResourceInfo/ResourceTag.h"
#include "../ResourceInfo/ResourceData.h"

namespace sip
{
	ToggleEnableCommand::ToggleEnableCommand(const CreateInfo& info) noexcept
		: info_{ info }
	{
	}

	ToggleEnableCommand::~ToggleEnableCommand() noexcept
	{
	}

	bool ToggleEnableCommand::execute() noexcept
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
		auto tag = section->getTag(info_.tag);
		if (tag == nullptr)
		{
			Logger << U"タグが存在しません。\n";
			return false;
		}
		auto resource = tag->getResource(info_.index);
		if (resource == nullptr)
		{
			Logger << U"リソースが存在しません。\n";
			return false;
		}
		resource->setEnable(!resource->isEnable());
		return true;
	}

	bool ToggleEnableCommand::undo() noexcept
	{
		return execute();
	}

	bool ToggleEnableCommand::redo() noexcept
	{
		return execute();
	}

	std::string ToggleEnableCommand::log() const noexcept
	{
		return "ToggleEnableCommand : \n"
			+ Unicode::Narrow(U"\t section : {}\n"_fmt(info_.section))
			+ Unicode::Narrow(U"\t tag     : {}\n"_fmt(info_.tag))
			+ Unicode::Narrow(U"\t index   : {}\n"_fmt(info_.index))
			;
	}
}
