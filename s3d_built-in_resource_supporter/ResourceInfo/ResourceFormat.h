#pragma once
#ifndef SIP_RESOURCE_FORMAT_H_
#define SIP_RESOURCE_FORMAT_H_

#include "ResourceDefine.h"

namespace sip
{
	class ResourceFormat
	{
	public:

		static String format(ResourceInfo* rc) noexcept;
	};
}

#endif // !SIP_RESOURCE_FORMAT_H_
