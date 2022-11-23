#pragma once
#ifndef SIP_RESOURCE_SECTION_H_
#define SIP_RESOURCE_SECTION_H_

#include "ResourceDefine.h"

namespace sip
{
	/// @brief 
	class ResourceSection
	{
	public:

		/// @brief  
		explicit ResourceSection() noexcept;

		/// @brief 
		~ResourceSection() noexcept;

		/// @brief 
		/// @param name  
		/// @return 
		[[nodiscard]] bool registTag(const String& name) noexcept;

		/// @brief 
		/// @param tag 
		/// @param index 
		/// @return 
		[[nodiscard]] bool insertTag(ResourceTagPtr&& tag, std::size_t index) noexcept;

		/// @brief 
		/// @param name 
		/// @param evacuation 
		/// @param erase_index 
		/// @return 
		[[nodiscard]] bool eraseTag(StringView name, ResourceTagPtr* evacuation = nullptr, std::size_t* erase_index = nullptr) noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] std::size_t getTagSize() const noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] ResourceTag* getTag() noexcept;

		/// @brief 
		/// @param name 
		/// @param regist  
		[[nodiscard]] ResourceTag* getTag(const String& name, bool regist = false) noexcept;

		/// @brief 
		/// @param index 
		/// @return 
		[[nodiscard]] ResourceTag* getTag(std::size_t index) noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] const ResourceTags& getTags() const noexcept;

		/// @brief 
		/// @return 
		const String& getName() const noexcept;

		/// @brief 
		/// @param name 
		void setName(StringView name) noexcept;

		/// @brief 
		/// @param name 
		/// @return 
		[[nodiscard]] ResourceTag* operator[](const String& name) noexcept;

	private:

		/// @brief 
		ResourceTagPtr tag_ptr_;

		/// @brief 
		ResourceTags tags_;

		/// @brief 
		String name_;
	};
}

#endif // !SIP_RESOURCE_SECTION_H_
