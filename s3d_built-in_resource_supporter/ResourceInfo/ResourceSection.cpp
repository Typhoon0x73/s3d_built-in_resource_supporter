#include "ResourceSection.h"
#include "ResourceTag.h"

namespace sip
{
	ResourceSection::ResourceSection() noexcept
		: tag_ptr_{ new ResourceTag() }
		, tags_{}
		, name_{ U"" }
	{
	}

	ResourceSection::~ResourceSection() noexcept
	{
	}

	bool ResourceSection::registTag(const String& name) noexcept
	{
		if (getTag(name))
		{
			Logger << name << U" : タグが存在します。\n";
			return false;
		}
		tags_.emplace_back(std::make_unique<ResourceTag>(name));
		return true;
	}

	bool ResourceSection::insertTag(ResourceTagPtr&& tag, std::size_t index) noexcept
	{
		if (tag == nullptr)
		{
			Logger << U"挿入するデータが nullptr です。\n";
			return false;
		}
		tags_.insert(tags_.begin() + index, std::move(tag));
		return true;
	}

	bool ResourceSection::eraseTag(StringView name, ResourceTagPtr* evacuation, std::size_t* erase_index) noexcept
	{
		std::size_t index = 0;
		for (auto it = tags_.begin(); it != tags_.end(); ++it, index++)
		{
			if ((*it)->getName().compare(name) != 0)
			{
				continue;
			}
			if (evacuation)
			{
				(*evacuation) = std::move(*it);
			}
			if (erase_index)
			{
				(*erase_index) = index;
			}
			tags_.erase(it);
			return true;
		}
		Logger << name << U" : タグが存在しませんでした。\n";
		return false;
	}

	std::size_t ResourceSection::getTagSize() const noexcept
	{
		return tags_.size();
	}

	ResourceTag* ResourceSection::getTag() noexcept
	{
		return tag_ptr_.get();
	}

	ResourceTag* ResourceSection::getTag(const String& name, bool regist) noexcept
	{
		if (name.compare(U"") == 0)
		{
			return getTag();
		}
		for (auto& tag : tags_)
		{
			if (tag->getName().compare(name) != 0)
			{
				continue;
			}
			return tag.get();
		}
		if (regist)
		{
			if (!registTag(name))
			{
				Logger << name << U" : タグの登録に失敗しました。\n";
				return nullptr;
			}
			return getTag(name);
		}
		return nullptr;
	}

	ResourceTag* ResourceSection::getTag(std::size_t index) noexcept
	{
		if (tags_.size() <= index)
		{
			Logger << index << U" : 配列の境界外を参照しています。\n";
			return nullptr;
		}
		return tags_[index].get();
	}

	const ResourceTags& ResourceSection::getTags() const noexcept
	{
		return tags_;
	}

	const String& ResourceSection::getName() const noexcept
	{
		return name_;
	}

	void ResourceSection::setName(StringView name) noexcept
	{
		name_ = name;
	}

	ResourceTag* ResourceSection::operator[](const String& name) noexcept
	{
		return getTag(name);
	}
}
