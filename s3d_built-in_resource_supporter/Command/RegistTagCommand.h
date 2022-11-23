#pragma once
#ifndef SIP_REGIST_TAG_COMMAND_H_
#define SIP_REGIST_TAG_COMMAND_H_

#include "ICommand.h"

namespace sip
{
	/// @brief 
	class RegistTagCommand : public ICommand
	{
	public:

		/// @brief 
		struct CreateInfo
		{
			/// @brief 
			std::size_t section;

			/// @brief 
			String tag;
		};

	public:

		/// @brief 
		/// @param info 
		explicit RegistTagCommand(const CreateInfo& info) noexcept;

		/// @brief 
		virtual ~RegistTagCommand() noexcept override;

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
	};
}

#endif // !SIP_REGIST_TAG_COMMAND_H_
