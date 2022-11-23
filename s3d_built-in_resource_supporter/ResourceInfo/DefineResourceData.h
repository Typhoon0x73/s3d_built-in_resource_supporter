#pragma once
#ifndef SIP_DEFINE_RESOURCE_DATA_H_
#define SIP_DEFINE_RESOURCE_DATA_H_

#include "ResourceData.h"

namespace sip
{
	/// @brief 
	class DefineResourceData : public ResourceData
	{
	public:

		/// @brief 
		/// @param id 
		/// @param define 
		/// @param path 
		/// @param enable 
		explicit DefineResourceData(std::uint64_t id, StringView define, FilePathView path, bool enable) noexcept;

		/// @brief 
		virtual ~DefineResourceData() noexcept override = default;

		/// @brief 
		/// @return 
		[[nodiscard]] std::uint64_t getID() const noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] const String& getDefine() const noexcept;

		/// @brief 
		/// @param id  
		void setID(std::uint64_t id) noexcept;

		/// @brief 
		/// @param define 
		void setDefine(StringView define) noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] virtual String format() const noexcept override;

	protected:

		/// @brief 
		std::uint64_t id_;

		/// @brief 
		String  define_;
	};
}

#endif // !SIP_DEFINE_RESOURCE_DATA_H_
