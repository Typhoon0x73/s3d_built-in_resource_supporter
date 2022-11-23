#pragma once
#ifndef SIP_XML_ATTRIBUTE_H_
#define SIP_XML_ATTRIBUTE_H_

#include "XMLParamDefine.h"

namespace sip
{

	/// @brief 
	class XMLAttribute
	{
	public:

		/// @brief 
		explicit XMLAttribute() noexcept;

		/// @brief 
		/// @param name 
		/// @param value 
		explicit XMLAttribute(StringView name, StringView value) noexcept;

		/// @brief 
		~XMLAttribute() noexcept;

		/// @brief 
		/// @return 
		const String& getName() const noexcept;

		/// @brief 
		/// @return 
		const String& getValue() const noexcept;

		/// @brief 
		/// @param name 
		void setName(StringView name) noexcept;

		/// @brief 
		/// @param value 
		void setValue(StringView value) noexcept;

		/// @brief 
		/// @return 
		String format() const noexcept;

	private:

		/// @brief 
		String name_;

		/// @brief 
		String value_;
	};
}

#endif // !SIP_XML_ATTRIBUTE_H_
