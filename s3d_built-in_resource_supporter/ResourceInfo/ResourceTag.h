#pragma once
#ifndef SIP_RESOURCE_TAG_H_
#define SIP_RESOURCE_TAG_H_

#include "ResourceDefine.h"

namespace sip
{
	/// @brief 
	class ResourceTag
	{
	public:

		/// @brief  
		explicit ResourceTag() noexcept;

		/// @brief 
		/// @param name 
		explicit ResourceTag(StringView name) noexcept;

		/// @brief
		~ResourceTag() noexcept;

		/// @brief 
		/// @param resource  
		void regist(ResourceData* resource) noexcept;

		/// @brief 
		/// @param data 
		/// @param index 
		/// @return 
		[[nodiscard]] bool insert(ResourceDataPtr&& data, std::size_t index) noexcept;

		/// @brief 
		/// @param index 
		/// @return 
		[[nodiscard]] ResourceDataPtr&& moveResource(std::size_t index) noexcept;

		/// @brief 
		/// @param n1 
		/// @param n2
		/// @return 
		[[nodiscard]] bool swap(std::size_t n1, std::size_t n2) noexcept;

		/// @brief 
		/// @param path 
		void erase(FilePathView path) noexcept;

		/// @brief 
		/// @param index  
		void erase(std::size_t index) noexcept;

		/// @brief 
		/// @param index 
		/// @return 
		[[nodiscard]] ResourceData* getResource(std::size_t index) noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] const ResourceDatas& getResources() const noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] const String& getName() const noexcept;

		/// @brief 
		/// @param name 
		void setName(StringView name) noexcept;

	private:

		/// @brief 
		ResourceDatas resources_;

		/// @brief 
		String name_;
	};
}

#endif // !SIP_RESOURCE_TAG_H_
