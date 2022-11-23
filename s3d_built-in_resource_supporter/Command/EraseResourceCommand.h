#pragma once
#ifndef SIP_ERASE_RESOURCE_COMMAND_H_
#define SIP_ERASE_RESOURCE_COMMAND_H_

#include "RegistResourceCommand.h"

namespace sip
{
	/// @brief 
	class EraseResourceCommand : public RegistResourceCommand
	{
	public:

		/// @brief 
		using CreateInfo = RegistResourceCommand::CreateInfo;

	public:

		/// @brief 
		explicit EraseResourceCommand(const CreateInfo& info) noexcept;

		/// @brief 
		virtual ~EraseResourceCommand() noexcept override;

		/// @brief 
		/// @return 
		virtual bool execute() noexcept override;

		/// @brief 
		/// @return 
		virtual bool undo() noexcept override;

		/// @brief 
		/// @return 
		virtual bool redo() noexcept override;

		/// @brief 
		/// @return 
		virtual std::string log() const noexcept override;
	};
}

#endif // !SIP_ERASE_RESOURCE_COMMAND_H_
