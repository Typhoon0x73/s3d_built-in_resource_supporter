#pragma once
#ifndef SIP_REGIST_RESOURCE_COMMAND_H_
#define SIP_REGIST_RESOURCE_COMMAND_H_

#include "ICommand.h"
#include <Siv3D.hpp>

namespace sip
{
	class RegistResourceCommand : public ICommand
	{
	public:

		/// @brief 
		struct CreateInfo
		{
			/// @brief 
			std::size_t section{ 0 };

			/// @brief 
			String tag{ U"" };

			/// @brief 
			FilePath path{ U"" };

			/// @brief 
			FilePath filter{ U"" };

			/// @brief 
			String extensions{ U"" };
		};

	public:

		/// @brief 
		/// @param info 
		explicit RegistResourceCommand(const CreateInfo& info) noexcept;

		/// @brief 
		virtual ~RegistResourceCommand() noexcept override;

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

	protected:

		/// @brief 
		CreateInfo info_;

		/// @brief 
		bool has_regist_tag_;

		/// @brief 
		String created_guid_;
	};
}

#endif // !SIP_REGIST_RESOURCE_COMMAND_H_
