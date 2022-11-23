#include "ResourceFactory.h"
#include "ResourceData.h"
#include "DefineResourceData.h"

namespace sip
{
	ResourceData* sip::ResourceFactory::Create(FilePathView path, bool enable) noexcept
	{
		return new ResourceData(path, enable);
	}

	ResourceData* ResourceFactory::Create(std::uint64_t id, StringView define, FilePathView path, bool enable) noexcept
	{
		return new DefineResourceData(id, define, path, enable);
	}
}
