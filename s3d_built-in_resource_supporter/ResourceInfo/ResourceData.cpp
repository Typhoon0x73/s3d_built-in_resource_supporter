#include "ResourceData.h"

namespace sip
{
	ResourceData::ResourceData(FilePathView path, bool enable) noexcept
		: file_path_{ path }
		, type_{ ResourceDataType::Resource }
		, enable_{ enable }
	{
	}

	bool ResourceData::isEnable() const noexcept
	{
		return enable_;
	}

	FilePathView ResourceData::getPath() const noexcept
	{
		return file_path_;
	}

	ResourceDataType ResourceData::getResourceDataType() const noexcept
	{
		return type_;
	}

	void ResourceData::setPath(FilePathView path) noexcept
	{
		file_path_ = path;
	}

	void ResourceData::setType(ResourceDataType type) noexcept
	{
		type_ = type;
	}

	void ResourceData::setEnable(bool enable) noexcept
	{
		enable_ = enable;
	}

	String ResourceData::format() const noexcept
	{
		return file_path_;
	}
}
