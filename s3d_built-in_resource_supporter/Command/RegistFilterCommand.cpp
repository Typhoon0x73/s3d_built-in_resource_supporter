#include "RegistFilterCommand.h"
#include "../stdafx.h"
#include "../XMLInfo/EditFilters.h"

namespace sip
{
	RegistFilterCommand::RegistFilterCommand(const CreateInfo& info) noexcept
		: info_{ info }
	{
	}

	RegistFilterCommand::~RegistFilterCommand() noexcept
	{
	}

	bool RegistFilterCommand::execute() noexcept
	{
		auto edit_filters = g_pGetBlackboard(EditFilters* const)->get("edit_filters");
		if (edit_filters == nullptr)
		{
			Logger << U"フィルターデータの取得に失敗しました。\n";
			return false;
		}
		if (!edit_filters->addFilter(info_.filter_path, info_.guid, info_.extensions))
		{
			Logger << U"フィルターの追加に失敗しました。\n";
			return false;
		}
		return true;
	}

	bool RegistFilterCommand::undo() noexcept
	{
		auto edit_filters = g_pGetBlackboard(EditFilters* const)->get("edit_filters");
		if (edit_filters == nullptr)
		{
			Logger << U"フィルターデータの取得に失敗しました。\n";
			return false;
		}
		if (!edit_filters->eraseFilter(info_.filter_path))
		{
			Logger << U"フィルターの削除に失敗しました。\n";
			return false;
		}
		return true;
	}

	bool RegistFilterCommand::redo() noexcept
	{
		return execute();
	}

	std::string RegistFilterCommand::log() const noexcept
	{
		return "RegistFilterCommand : \n"
			+ Unicode::Narrow(U"\t filter     : {}\n"_fmt(info_.filter_path))
			+ Unicode::Narrow(U"\t guid       : {}\n"_fmt(info_.guid))
			+ Unicode::Narrow(U"\t extensions : {}\n"_fmt(info_.extensions))
			;
	}
}
