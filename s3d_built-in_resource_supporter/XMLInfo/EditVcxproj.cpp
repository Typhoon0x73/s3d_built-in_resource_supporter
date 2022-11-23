#include "EditVcxproj.h"
#include "XMLAttribute.h"
#include "XMLElement.h"
#include "XMLData.h"

namespace sip
{
	EditVcxproj::EditVcxproj() noexcept
		: item_groups_{}
		, item_groups_parent_{ nullptr }
		, project_guid_{ U"" }
		, resource_path_{ U"" }
	{
	}

	EditVcxproj::~EditVcxproj() noexcept
	{
	}

	bool EditVcxproj::addItemGroup(StringView name) noexcept
	{
		if (findItemGroup(name))
		{
			Logger << name << U" : アイテムグループは存在します。\n";
			return false;
		}
		auto data = std::make_shared<XMLData>();
		data->getElement()->setTag(U"ItemGroup");
		item_groups_.emplace_back(std::make_pair(name, data));
		item_groups_parent_->insertChild(name, std::move(data), insert_point_++);
		return true;
	}

	bool EditVcxproj::insertItemGroup(StringView name, XMLDataPtr evacuation, std::size_t parent_erase_index, std::size_t itemgroup_erase_index) noexcept
	{
		if (evacuation == nullptr)
		{
			Logger << U"挿入データが nullptr です。";
			return false;
		}
		item_groups_parent_->insertChild(name, evacuation, parent_erase_index);
		item_groups_.insert(item_groups_.begin() + itemgroup_erase_index, std::make_pair(String(name), evacuation));
		insert_point_++;
		return true;
	}

	bool EditVcxproj::eraseItemGroup(StringView name
		, XMLDataPtr* evacuation
		, std::size_t* parent_erase_index
		, std::size_t* itemgroup_erase_index) noexcept
	{
		item_groups_parent_->eraseChildData(name, parent_erase_index);
		std::size_t index = 0;
		for (auto it = item_groups_.begin(); it != item_groups_.end(); ++it, index++)
		{
			if ((*it).first.compare(name) != 0)
			{
				continue;
			}
			if (evacuation)
			{
				(*evacuation) = (*it).second;
			}
			if (itemgroup_erase_index)
			{
				(*itemgroup_erase_index) = index;
			}
			insert_point_--;
			item_groups_.erase(it);
			return true;
		}
		Logger << name << U" : アイテムグループは存在しませんでした。\n";
		return false;
	}

	bool EditVcxproj::addItem(StringView name, FilePathView path) noexcept
	{
		auto item_group = findItemGroup(name);
		if (item_group == nullptr)
		{
			if (addItemGroup(name))
			{
				item_group = findItemGroup(name);
				if (item_group == nullptr)
				{
					Logger << name << U" : アイテムグループは存在しません。\n";
					return false;
				}
			}
		}
		XMLDataPtr item = std::make_shared<XMLData>();
		item->getElement()->setTag(name);
		item->getElement()->registAttribute(U"Include", Unicode::Widen(path.toUTF8()));
		item_group->registChild(std::move(item));
		return true;
	}

	bool EditVcxproj::eraseItem(StringView name, FilePathView path) noexcept
	{
		auto item_group = findItemGroup(name);
		if (item_group == nullptr)
		{
			Logger << name << U" : アイテムグループは存在しません。\n";
			return false;
		}
		item_group->eraseChildData([&](XMLData* pData) -> bool
			{
				if (pData->getElement()->getTag().compare(name) != 0)
				{
					return false;
				}
				auto attribute = pData->getElement()->findAttribute(U"Include");
				if (attribute == nullptr)
				{
					return false;
				}
				if (attribute->getValue().compare(path) != 0)
				{
					return false;
				}
				return true;
			}
		);
		return true;
	}

	XMLData* EditVcxproj::findItemGroup(StringView name) noexcept
	{
		for (auto& item_group : item_groups_)
		{
			if (item_group.first.compare(name) != 0)
			{
				continue;
			}
			return item_group.second.get();
		}
		return nullptr;
	}

	void EditVcxproj::set(XMLInfo& info) noexcept
	{
		insert_point_ = 0;
		item_groups_.clear();
		auto root = info.getRoot();
		if (root == nullptr)
		{
			Logger << U"親データが nullptr です。\n";
			return;
		}
		auto guid_element = root->findElement(U"ProjectGuid");
		if (guid_element)
		{
			project_guid_ = guid_element->getValue();
		}
		item_groups_parent_ = root;
		std::size_t end_point = 0;
		for (auto& data : root->getChildren())
		{
			bool is_item_group   = data.second->getElement()->getTag().compare(U"ItemGroup") == 0;
			bool is_no_attribute = data.second->getElement()->getAttributes().size() == 0;
			if (is_item_group && is_no_attribute)
			{
				item_groups_.emplace_back(data);
				insert_point_ = end_point;
				auto resource = data.second->findElement(U"ResourceCompile");
				if (resource != nullptr)
				{
					auto attribute = resource->findAttribute(U"Include");
					if (attribute != nullptr)
					{
						resource_path_ = attribute->getValue();
					}
				}
			}
			end_point++;
		}
	}

	const String& EditVcxproj::getProjectGUID() const noexcept
	{
		return project_guid_;
	}

	const FilePath& EditVcxproj::getResourcePath() const noexcept
	{
		return resource_path_;
	}
}
