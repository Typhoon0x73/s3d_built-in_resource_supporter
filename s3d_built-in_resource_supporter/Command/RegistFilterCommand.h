#pragma once
#ifndef SIP_REGIST_FILTER_COMMAND_H_
#define SIP_REGIST_FILTER_COMMAND_H_

#include "ICommand.h"
#include <Siv3D.hpp>

namespace sip
{
	/// @brief 
	class RegistFilterCommand : public ICommand
	{
	public:

		/// @brief 
		struct CreateInfo
		{
			/// @brief 
			FilePath filter_path;

			/// @brief 
			String guid;

			/// @brief 
			String extensions;
		};

	public:

		/// @brief 
		/// @param info 
		explicit RegistFilterCommand(const CreateInfo& info) noexcept;

		/// @brief 
		virtual ~RegistFilterCommand() noexcept override;

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

#endif // !SIP_REGIST_FILTER_COMMAND_H_
