#pragma once
#ifndef SIP_MOVE_TAG_COMMAND_H_
#define SIP_MOVE_TAG_COMMAND_H_

#include "ICommand.h"
#include <Siv3D.hpp>

namespace sip
{
	/// @brief 
	class MoveTagCommand : public ICommand
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
			std::size_t prev_point;

			/// @brief 
			std::size_t next_point;
		};

	public:

		/// @brief 
		/// @param info 
		explicit MoveTagCommand(const CreateInfo& info) noexcept;

		/// @brief
		virtual ~MoveTagCommand() noexcept override;

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

#endif // !SIP_MOVE_TAG_COMMAND_H_
