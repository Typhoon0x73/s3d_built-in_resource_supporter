#pragma once
#ifndef SIP_ERASE_FILTER_COMMAND_H_
#define SIP_ERASE_FILTER_COMMAND_H_

#include "ICommand.h"
#include <Siv3D.hpp>

namespace sip
{
	/// @brief 
	class EraseFilterCommand : public ICommand
	{
	public:

		/// @brief 
		struct CreateInfo
		{
			/// @brief 
			FilePath filter_path;
		};

	public:

		/// @brief 
		/// @param info 
		explicit EraseFilterCommand(const CreateInfo& info) noexcept;

		/// @brief 
		virtual ~EraseFilterCommand() noexcept override;

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

	private:

		/// @brief 
		CreateInfo info_;

		/// @brief 
		String guid_;

		/// @brief 
		String extensions_;
	};
}

#endif // !SIP_ERASE_FILTER_COMMAND_H_
