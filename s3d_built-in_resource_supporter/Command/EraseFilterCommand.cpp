#include "EraseFilterCommand.h"
#include "../stdafx.h"
#include "../XMLInfo/EditFilters.h"
#include "../XMLInfo/XMLData.h"
#include "../XMLInfo/XMLElement.h"

namespace sip
{
	EraseFilterCommand::EraseFilterCommand(const CreateInfo& info) noexcept
		: info_{ info }
		, guid_{ U"" }
		, extensions_{ U"" }
	{
	}

	EraseFilterCommand::~EraseFilterCommand() noexcept
	{
	}

	bool EraseFilterCommand::execute() noexcept
	{
		auto edit_filters = g_pGetBlackboard(EditFilters* const)->get("edit_filters");
		if (edit_filters == nullptr)
		{
			Logger << U"フィルターデータの取得に失敗しました。\n";
			return false;
		}
		if (edit_filters->existFilterReference(info_.filter_path))
		{
			Logger << info_.filter_path << U" : フィルターの参照が存在します。\n";
			return false;
		}
		auto filter = edit_filters->findFilter(info_.filter_path);
		if (filter == nullptr)
		{
			Logger << U"フィルターが存在しません。\n";
			return false;
		}
		if (auto extensions = filter->findElement(U"Extensions"))
		{
			extensions_ = extensions->getValue();
		}
		if (auto guid = filter->findElement(U"UniqueIdentifier"))
		{
			guid_ = guid->getValue();
		}
		if (!edit_filters->eraseFilter(info_.filter_path))
		{
			Logger << U"フィルターの削除に失敗しました。\n";
			return false;
		}
		return true;
	}

	bool EraseFilterCommand::undo() noexcept
	{
		auto edit_filters = g_pGetBlackboard(EditFilters* const)->get("edit_filters");
		if (edit_filters == nullptr)
		{
			Logger << U"フィルターデータの取得に失敗しました。\n";
			return false;
		}
		if (!edit_filters->addFilter(info_.filter_path, guid_, extensions_))
		{
			Logger << U"フィルターの追加に失敗しました。\n";
			return false;
		}
		return true;
	}

	bool EraseFilterCommand::redo() noexcept
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
		return false;
	}

	std::string EraseFilterCommand::log() const noexcept
	{
		return "EraseFilterCommand : \n"
			+ Unicode::Narrow(U"\t filter     : {}\n"_fmt(info_.filter_path))
			+ Unicode::Narrow(U"\t guid       : {}\n"_fmt(guid_))
			+ Unicode::Narrow(U"\t extensions : {}\n"_fmt(extensions_))
			;
	}
}
