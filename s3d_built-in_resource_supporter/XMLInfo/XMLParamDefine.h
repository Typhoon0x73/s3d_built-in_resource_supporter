#pragma once
#ifndef SIP_XML_PARAM_DEFINE_H_
#define SIP_XML_PARAM_DEFINE_H_

#include <Siv3D.hpp>

namespace sip
{
	class XMLAttribute;
	class XMLElement;
	class XMLData;
	using XMLAttributePtr = std::shared_ptr<XMLAttribute>;
	using XMLElementPtr   = std::shared_ptr<XMLElement>;
	using XMLDataPtr      = std::shared_ptr<XMLData>;
	using XMLAttributes   = std::vector<XMLAttributePtr>;
	using XMLElements     = std::vector<XMLElementPtr>;
	using XMLDatas        = std::vector<std::pair<String, XMLDataPtr>>;
	using XMLDataFindFunc = std::function<bool(XMLData*)>;
	using FilterData      = std::pair<FilePath, String>;
	using FilterDatas     = std::vector<FilterData>;
}

#endif // !SIP_XML_PARAM_DEFINE_H_
