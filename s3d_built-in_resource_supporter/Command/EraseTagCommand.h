#pragma once
#ifndef SIP_ERASE_TAG_COMMAND_H_
#define SIP_ERASE_TAG_COMMAND_H_

#include "ICommand.h"
#include "../ResourceInfo/ResourceDefine.h"
#include "../XMLInfo/XMLInfo.h"

namespace sip
{
	/// @brief 
	class EraseTagCommand : public ICommand
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
		explicit EraseTagCommand(const CreateInfo& info) noexcept;

		/// @brief 
		virtual ~EraseTagCommand() noexcept override;

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
		std::size_t rc_erase_index_;

		/// @brief 
		ResourceTagPtr rc_evacuation_;

		/// @brief 
		std::size_t vcxproj_erase_index_;

		/// @brief 
		std::size_t vcxproj_parent_erase_index_;

		/// @brief 
		XMLDataPtr vcxproj_evacuation_;

		/// @brief 
		std::size_t filters_erase_index_;

		/// @brief 
		std::size_t filters_parent_erase_index_;

		/// @brief 
		XMLDataPtr filters_evacuation_;
	};
}

#endif // !SIP_ERASE_TAG_COMMAND_H_
