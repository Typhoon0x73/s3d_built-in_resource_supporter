#include "ResourceTag.h"
#include "ResourceData.h"
#include "DefineResourceData.h"

namespace sip
{
	ResourceTag::ResourceTag() noexcept
		: resources_{}
		, name_{ U"" }
	{
	}

	ResourceTag::ResourceTag(StringView name) noexcept
		: resources_{}
		, name_{ name }
	{
	}

	ResourceTag::~ResourceTag() noexcept
	{
		resources_.clear();
	}

	void ResourceTag::regist(ResourceData* resource) noexcept
	{
		resources_.emplace_back(resource);
	}

	bool ResourceTag::insert(ResourceDataPtr&& data, std::size_t index) noexcept
	{
		if (data == nullptr)
		{
			Logger << U"挿入するデータが nullptr です。\n";
			return false;
		}
		if (index > resources_.size())
		{
			Logger << index << U" : 挿入する位置が配列の境界外です。\n";
			return false;
		}
		resources_.insert(resources_.begin() + index, std::move(data));
		return true;
	}

	ResourceDataPtr&& ResourceTag::moveResource(std::size_t index) noexcept
	{
		if (index >= resources_.size())
		{
			Logger << index << U" : 配列の境界外です。\n";
			return std::move(nullptr);
		}
		ResourceDataPtr&& data(std::move(resources_[index]));
		erase(index);
		return std::move(data);
	}

	bool ResourceTag::swap(std::size_t n1, std::size_t n2) noexcept
	{
		if (n1 == n2)
		{
			return true;
		}
		if (resources_.size() <= n1 || resources_.size() <= n2)
		{
			Logger << n1 << U" or " << n2 << U" : 配列の境界外です。\n";
			return false;
		}
		std::swap(resources_[n1], resources_[n2]);
		return true;
	}

	void ResourceTag::erase(FilePathView path) noexcept
	{
		for (auto it = resources_.begin(); it != resources_.end(); ++it)
		{
			if ((*it)->getPath().compare(path) != 0)
			{
				continue;
			}
			resources_.erase(it);
			break;
		}
	}

	void ResourceTag::erase(std::size_t index) noexcept
	{
		resources_.erase(resources_.begin() + index);
	}

	ResourceData* ResourceTag::getResource(std::size_t index) noexcept
	{
		if (index >= resources_.size())
		{
			Logger << index << U" : 配列の境界外です。\n";
			return nullptr;
		}
		return resources_[index].get();
	}

	const ResourceDatas& ResourceTag::getResources() const noexcept
	{
		return resources_;
	}

	const String& ResourceTag::getName() const noexcept
	{
		return name_;
	}

	void ResourceTag::setName(StringView name) noexcept
	{
		name_ = name;
	}
}
