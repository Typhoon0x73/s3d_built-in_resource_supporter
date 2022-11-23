#pragma once
#ifndef SIP_RESOURCE_FACTORY_H_
#define SIP_RESOURCE_FACTORY_H_

#include "ResourceDefine.h"

namespace sip
{
	/// @brief 
	class ResourceFactory
	{
	public:
		/// @brief 
		/// @param path 
		/// @param enable 
		/// @return 
		[[nodiscard]] static ResourceData* Create(FilePathView path, bool enable) noexcept;

		/// @brief 
		/// @param id 
		/// @param define 
		/// @param path 
		/// @param enable 
		/// @return 
		[[nodiscard]] static ResourceData* Create(std::uint64_t id, StringView define, FilePathView path, bool enable) noexcept;
	};
}

#endif // !SIP_RESOURCE_FACTORY_H_
