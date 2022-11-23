#pragma once
#ifndef SIP_RESOURCE_PARSER_H_
#define SIP_RESOURCE_PARSER_H_

#include "ResourceDefine.h"

namespace sip
{
	class ResourceParser
	{
	public:

		static bool parse(const String& text, ResourceInfo* rc) noexcept;
	};
}

#endif // !SIP_RESOURCE_PARSER_H_
