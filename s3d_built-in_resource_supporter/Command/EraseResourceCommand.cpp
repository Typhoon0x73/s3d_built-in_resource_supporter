#include "EraseResourceCommand.h"

namespace sip
{
	EraseResourceCommand::EraseResourceCommand(const CreateInfo& info) noexcept
		: RegistResourceCommand(info)
	{
	}

	EraseResourceCommand::~EraseResourceCommand() noexcept
	{
	}

	bool EraseResourceCommand::execute() noexcept
	{
		return RegistResourceCommand::undo();
	}

	bool EraseResourceCommand::undo() noexcept
	{
		return RegistResourceCommand::execute();
	}

	bool EraseResourceCommand::redo() noexcept
	{
		return RegistResourceCommand::undo();
	}

	std::string EraseResourceCommand::log() const noexcept
	{
		return "EraseResourceCommand : \n"
			+ Unicode::Narrow(U"\t section    : {}\n"_fmt(info_.section))
			+ Unicode::Narrow(U"\t tag        : {}\n"_fmt(info_.tag))
			+ Unicode::Narrow(U"\t path       : {}\n"_fmt(info_.path))
			+ Unicode::Narrow(U"\t filter     : {}\n"_fmt(info_.filter))
			+ Unicode::Narrow(U"\t extensions : {}\n"_fmt(info_.extensions))
			;
	}
}
