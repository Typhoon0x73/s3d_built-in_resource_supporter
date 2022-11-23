#pragma once
#ifndef SIP_FILTERS_INFO_H_
#define SIP_FILTERS_INFO_H_

#include "XMLInfo.h"

namespace sip
{
	/// @brief 
	class EditFilters
	{
	public:

		/// @brief 
		explicit EditFilters() noexcept;

		/// @brief 
		~EditFilters() noexcept;

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
		/// @param path 
		/// @param guid 
		/// @param extensions 
		/// @return 
		[[nodiscard]] bool addFilter(FilePathView path, StringView guid, StringView extensions = U"") noexcept;

		/// @brief 
		/// @param name 
		/// @param path 
		/// @param filter 
		/// @return 
		[[nodiscard]] bool addItem(StringView name, FilePathView path, FilePathView filter = U"") noexcept;

		/// @brief 
		/// @param name 
		/// @param evacuation 
		/// @param parent_erase_index 
		/// @param itemgroup_erase_index 
		/// @return 
		[[nodiscard]] bool eraseItemGroup(StringView name, XMLDataPtr* evacuation = nullptr,
			std::size_t* parent_erase_index = nullptr, std::size_t* itemgroup_erase_index = nullptr) noexcept;

		/// @brief 
		/// @param path 
		/// @return 
		[[nodiscard]] bool eraseFilter(FilePathView path) noexcept;

		/// @brief 
		/// @param name 
		/// @param path 
		/// @return 
		[[nodiscard]] bool eraseItem(StringView name, FilePathView path) noexcept;

		/// @brief 
		/// @param path 
		/// @return 
		[[nodiscard]] XMLData* findFilter(FilePathView path) const noexcept;

		/// @brief 
		/// @param path 
		/// @return 
		[[nodiscard]] bool existFilter(FilePathView path) const noexcept;

		/// @brief 
		/// @param guid 
		/// @return 
		[[nodiscard]] bool existGUID(StringView guid) const noexcept;

		/// @brief 
		/// @param name 
		/// @return 
		[[nodiscard]] XMLData* findItemGroup(StringView name) noexcept;

		/// @brief 
		/// @param info 
		void set(XMLInfo& info) noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] XMLData* getFilters() noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] const FilterDatas& getGUIDs() const noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] const XMLDatas& getItemGroups() const noexcept;

		/// @brief 
		/// @param path 
		/// @return 
		[[nodiscard]] bool existFilterReference(FilePathView path) const noexcept;

	private:

		/// @brief 
		XMLData* item_groups_parent_;

		/// @brief 
		XMLDataPtr filters_;

		/// @brief 
		FilterDatas guids_;

		/// @brief 
		XMLDatas item_groups_;

		/// @brief 
		std::size_t insert_point_;
	};
}

#endif // !SIP_FILTERS_INFO_H_
