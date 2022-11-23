#pragma once
#ifndef SIP_TOGGLE_ENABLE_COMMAND_H_
#define SIP_TOGGLE_ENABLE_COMMAND_H_

#include "ICommand.h"
#include <Siv3D.hpp>

namespace sip
{
	/// @brief 
	class ToggleEnableCommand : public ICommand
	{
	public:

		/// @brief 
		struct CreateInfo
		{
			/// @brief 
			std::size_t section;

			/// @brief 
			String tag;

			/// @brief 
			std::size_t index;
		};

	public:

		/// @brief 
		/// @param info 
		explicit ToggleEnableCommand(const CreateInfo& info) noexcept;

		/// @brief
		virtual ~ToggleEnableCommand() noexcept override;

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

#endif // !SIP_TOGGLE_ENABLE_COMMAND_H_
