#pragma once
#ifndef SIP_MOVE_RESOURCE_COMMAND_H_
#define SIP_MOVE_RESOURCE_COMMAND_H_

#include "ICommand.h"
#include <Siv3D.hpp>

namespace sip
{
	/// @brief 
	class MoveResourceCommand : public ICommand
	{
	public:

		/// @brief 
		struct CreateInfo
		{
			/// @brief 
			std::size_t section;

			/// @brief 
			String prev_tag;

			/// @brief 
			String next_tag;

			/// @brief 
			std::size_t prev_index;

			/// @brief 
			std::size_t next_index;
		};

	public:

		/// @brief 
		/// @param info 
		explicit MoveResourceCommand(const CreateInfo& info) noexcept;

		/// @brief
		virtual ~MoveResourceCommand() noexcept override;

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

#endif // !SIP_MOVE_RESOURCE_COMMAND_H_
