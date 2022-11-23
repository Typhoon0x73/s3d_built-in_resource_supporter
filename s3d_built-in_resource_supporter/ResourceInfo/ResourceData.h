#pragma once
#ifndef SIP_RESOURCE_DATA_H_
#define SIP_RESOURCE_DATA_H_

#include "ResourceDefine.h"

namespace sip
{
	/// @brief 
	class ResourceData
	{
	public:

		/// @brief 
		/// @param path  
		explicit ResourceData(FilePathView path, bool enable) noexcept;

		/// @brief 
		virtual ~ResourceData() noexcept = default;

		/// @brief 
		/// @return 
		[[nodiscard]] bool isEnable() const noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] FilePathView getPath() const noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] ResourceDataType getResourceDataType() const noexcept;

		/// @brief 
		/// @param path  
		void setPath(FilePathView path) noexcept;

		/// @brief 
		/// @param type 
		void setType(ResourceDataType type) noexcept;

		/// @brief 
		/// @param enable 
		void setEnable(bool enable) noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] virtual String format() const noexcept;

	protected:

		/// @brief 
		ResourceDataType type_;

		/// @brief 
		FilePath file_path_;

		/// @brief 
		bool enable_;
	};
}

#endif // !SIP_RESOURCE_DATA_H_
