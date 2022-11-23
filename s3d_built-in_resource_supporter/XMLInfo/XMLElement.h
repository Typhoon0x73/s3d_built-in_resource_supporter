#pragma once
#ifndef SIP_XML_ELEMENT_H_
#define SIP_XML_ELEMENT_H_

#include "XMLParamDefine.h"

namespace sip
{
	/// @brief 
	class XMLElement
	{
	public:

		/// @brief 
		explicit XMLElement() noexcept;

		/// @brief
		~XMLElement() noexcept;

		/// @brief 
		void clear() noexcept;

		/// @brief 
		/// @param name 
		/// @param value 
		void registAttribute(StringView name, StringView value) noexcept;

		/// @brief 
		/// @param name 
		/// @return 
		XMLAttribute* findAttribute(StringView name) noexcept;

		/// @brief 
		/// @param index 
		/// @return 
		XMLAttribute* getAttribute(std::size_t index) noexcept;

		/// @brief 
		/// @return 
		const XMLAttributes& getAttributes() const noexcept;

		/// @brief 
		/// @return 
		const String& getTag() const noexcept;

		/// @brief 
		/// @return 
		const String& getValue() const noexcept;

		/// @brief 
		/// @param tag 
		void setTag(StringView tag) noexcept;

		/// @brief 
		/// @param value 
		void setValue(StringView value) noexcept;

	private:

		/// @brief 
		String tag_;

		/// @brief 
		String value_;

		/// @brief 
		XMLAttributes attributes_;
	};
}

#endif // !SIP_XML_ELEMENT_H_
