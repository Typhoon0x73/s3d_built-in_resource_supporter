#include "DefineResourceData.h"

namespace sip
{
	sip::DefineResourceData::DefineResourceData(std::uint64_t id, StringView define, FilePathView path, bool enable) noexcept
		: ResourceData{ path, enable }
		, id_{ id }
		, define_{ define }
	{
		setType(ResourceDataType::DefineResource);
	}

	std::uint64_t DefineResourceData::getID() const noexcept
	{
		return id_;
	}

	const String& DefineResourceData::getDefine() const noexcept
	{
		return define_;
	}

	void DefineResourceData::setID(std::uint64_t id) noexcept
	{
		id_ = id;
	}

	void DefineResourceData::setDefine(StringView define) noexcept
	{
		define_ = define;
	}

	String DefineResourceData::format() const noexcept
	{
		return U"{}, {}, {}"_fmt(id_, define_, file_path_);
	}
}
