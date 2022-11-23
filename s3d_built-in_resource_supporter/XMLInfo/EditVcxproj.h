#pragma once
#ifndef SIP_VCXPROJ_INFO_H_
#define SIP_VCXPROJ_INFO_H_

#include "XMLInfo.h"

namespace sip
{
	/// @brief 
	class EditVcxproj
	{
	public:

		/// @brief 
		explicit EditVcxproj() noexcept;

		/// @brief 
		~EditVcxproj() noexcept;

		/// @brief 
		/// @param name 
		/// @return 
		[[nodiscard]] bool addItemGroup(StringView name) noexcept;

		/// @brief 
		/// @param name 
		/// @param evacuation 
		/// @param parent_erase_index 
		/// @param itemgroup_erase_index 
		/// @return 
		[[nodiscard]] bool insertItemGroup(StringView name, XMLDataPtr evacuation, std::size_t parent_erase_index, std::size_t itemgroup_erase_index) noexcept;

		/// @brief 
		/// @param name 
		/// @param evacuation 
		/// @param parent_erase_index 
		/// @param itemgroup_erase_index 
		/// @return 
		[[nodiscard]] bool eraseItemGroup(StringView name, XMLDataPtr* evacuation = nullptr,
			std::size_t* parent_erase_index = nullptr, std::size_t* itemgroup_erase_index = nullptr) noexcept;

		/// @brief 
		/// @param name 
		/// @param path 
		/// @return 
		[[nodiscard]] bool addItem(StringView name, FilePathView path) noexcept;

		/// @brief 
		/// @param name 
		/// @param path 
		/// @return 
		[[nodiscard]] bool eraseItem(StringView name, FilePathView path) noexcept;

		/// @brief 
		/// @param name 
		/// @return 
		[[nodiscard]] XMLData* findItemGroup(StringView name) noexcept;

		/// @brief 
		/// @param info 
		void set(XMLInfo& info) noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] const String& getProjectGUID() const noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] const FilePath& getResourcePath() const noexcept;

	private:

		/// @brief 
		XMLData* item_groups_parent_;

		/// @brief 
		XMLDatas item_groups_;

		/// @brief 
		String project_guid_;

		/// @brief 
		FilePath resource_path_;

		/// @brief 
		std::size_t insert_point_;
	};
}

#endif // !SIP_VCXPROJ_INFO_H_
