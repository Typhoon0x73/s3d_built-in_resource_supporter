#pragma once
#ifndef SIP_XML_DATA_H_
#define SIP_XML_DATA_H_

#include "XMLParamDefine.h"

namespace sip
{

	/// @brief 
	class XMLData : public std::enable_shared_from_this<XMLData>
	{
	public:

		/// @brief 
		explicit XMLData() noexcept;

		/// @brief
		~XMLData() noexcept;

		/// @brief 
		void clear() noexcept;

		/// @brief 
		/// @param data 
		void registChild(XMLDataPtr&& data) noexcept;

		/// @brief 
		/// @param name 
		/// @param data 
		/// @param index 
		void insertChild(StringView name, XMLDataPtr data, std::size_t index) noexcept;

		/// @brief 
		/// @param func 
		/// @param erase_index 
		void eraseChildData(const XMLDataFindFunc& func, std::size_t* erase_index = nullptr) noexcept;

		/// @brief 
		/// @param name 
		/// @param erase_index 
		void eraseChildData(StringView name, std::size_t* erase_index = nullptr) noexcept;

		/// @brief 
		/// @param tag 
		/// @return 
		[[nodiscard]] XMLDataPtr findData(StringView tag) noexcept;

		/// @brief 
		/// @param func 
		/// @return 
		[[nodiscard]] XMLDataPtr findData(const XMLDataFindFunc& func) noexcept;

		/// @brief 
		/// @param tag 
		/// @return 
		[[nodiscard]] XMLElement* findElement(StringView tag) noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] XMLElement* getElement() noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] const XMLElement* getElement() const noexcept;

		/// @brief 
		/// @param index 
		/// @return 
		[[nodiscard]] XMLData* getChild(std::size_t index) noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] const XMLDatas& getChildren() const noexcept;

	private:

		/// @brief 
		XMLElementPtr element_;

		/// @brief 
		XMLDatas children_;
	};
}

#endif // !SIP_XML_DATA_H_
