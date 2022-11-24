#pragma once
#ifndef SIP_RESOURCE_INFO_H_
#define SIP_RESOURCE_INFO_H_

#include "ResourceDefine.h"

namespace sip
{
	/// @brief 
	class ResourceInfo
	{
	public:

		/// @brief 
		explicit ResourceInfo() noexcept;

		/// @brief 
		~ResourceInfo() noexcept;

		/// @brief 
		void clear() noexcept;

		/// @brief 
		/// @param path 
		/// @return 
		[[nodiscard]] bool open(FilePathView path) noexcept;

		/// @brief 
		/// @param  
		/// @return 
		[[nodiscard]] bool save(FilePathView path) noexcept;

		/// @brief 
		/// @param name 
		/// @return 
		[[nodiscard]] bool registSection(StringView name) noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] std::size_t getSectionSize() const noexcept;

		/// @brief 
		/// @param index 
		/// @return 
		[[nodiscard]] ResourceSection* getSection(std::size_t index) noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] const ResourceSections& getSections() const noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] const String& getFooterComment() const noexcept;

		/// @brief 
		/// @param comment  
		void setFooterComment(const String& comment) noexcept;

	private:

		/// @brief 
		ResourceSections sections_;

		/// @brief 
		String footer_comment_;
	};
}

#endif // !SIP_RESOURCE_INFO_H_
