#pragma once
#ifndef SIP_XML_INFO_H_
#define SIP_XML_INFO_H_

#include "XMLParamDefine.h"

namespace sip
{
	/// @brief 
	class XMLInfo
	{
	public:

		/// @brief  
		explicit XMLInfo() noexcept;

		/// @brief
		~XMLInfo() noexcept;

		/// @brief 
		/// @param path 
		/// @return 
		bool save(FilePathView path) const noexcept;

		/// @brief  
		void clear() noexcept;

		/// @brief 
		/// @param element 
		void set(const s3d::XMLElement& element) noexcept;

		/// @brief 
		/// @return 
		const String& getVersion() const noexcept;

		/// @brief 
		/// @return 
		const String& getEncoding() const noexcept;

		/// @brief 
		/// @return 
		const XMLData* getRoot() const noexcept;

		/// @brief 
		/// @return 
		XMLData* getRoot() noexcept;

		/// @brief 
		/// @param mejor 
		/// @param miner 
		void setVersion(std::size_t mejor, std::size_t miner) noexcept;

		/// @brief 
		/// @param encoding 
		void setEncoding(StringView encoding) noexcept;

		/// @brief 
		/// @return 
		String format() const noexcept;

	private:

		/// @brief 
		/// @param data 
		/// @param element 
		void setElement(XMLData* data, const s3d::XMLElement& element) noexcept;

		/// @brief 
		/// @param data 
		/// @param tab_count 
		/// @return 
		String formatElement(const XMLData* data, std::size_t tab_count = 0) const noexcept;

	private:

		/// @brief 
		String version_;

		/// @brief 
		String encoding_;

		/// @brief 
		XMLDataPtr root_;
	};
}

#endif // !SIP_XML_INFO_H_
