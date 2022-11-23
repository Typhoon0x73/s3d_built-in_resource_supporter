#include "EditFilters.h"
#include "XMLAttribute.h"
#include "XMLElement.h"
#include "XMLData.h"

namespace sip
{
	namespace detail
	{
		bool isItemGroup(XMLElement* element)
		{
			return (element->getTag().compare(U"ItemGroup") == 0);
		}

		bool haveUniqueIdentifier(XMLData* data)
		{
			return (data->findData(U"UniqueIdentifier") != nullptr);
		}

		bool findFilterItemGroupFunc(XMLData* pData)
		{
			if (!isItemGroup(pData->getElement()))
			{
				return false;
			}
			if (pData->getChildren().size() == 0)
			{
				return false;
			}
			if (!haveUniqueIdentifier(pData->getChild(0)))
			{
				return false;
			}
			return true;
		}
	}

	EditFilters::EditFilters() noexcept
		: filters_{ nullptr }
		, item_groups_{}
		, guids_{}
		, item_groups_parent_{ nullptr }
	{
	}

	EditFilters::~EditFilters() noexcept
	{
		if (filters_)
		{
			filters_->clear();
			filters_.reset();
		}
		item_groups_.clear();
		guids_.clear();
	}

	bool EditFilters::addItemGroup(StringView name) noexcept
	{
		if (findItemGroup(name))
		{
			Logger << name << U" : アイテムグループは存在します。\n";
			return false;
		}
		auto data = std::make_shared<XMLData>();
		data->getElement()->setTag(U"ItemGroup");
		item_groups_.emplace_back(std::make_pair(String(name), data));
		item_groups_parent_->insertChild(name, std::move(data), insert_point_++);
		return true;
	}

	bool EditFilters::insertItemGroup(StringView name, XMLDataPtr evacuation, std::size_t parent_erase_index, std::size_t itemgroup_erase_index) noexcept
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

	bool EditFilters::addFilter(FilePathView path, StringView guid, StringView extensions) noexcept
	{
		if (filters_ == nullptr)
		{
			Logger << U"フィルターデータが nullptr です。\n";
			return false;
		}
		if (existGUID(guid))
		{
			Logger << guid << U" : GUIDは存在します。\n";
			return false;
		}
		XMLDataPtr filter = std::make_shared<XMLData>();
		XMLDataPtr unique = std::make_shared<XMLData>();
		filter->getElement()->setTag(U"Filter");
		unique->getElement()->setTag(U"UniqueIdentifier");
		unique->getElement()->setValue(guid);
		filter->getElement()->registAttribute(U"Include", Unicode::Widen(path.toUTF8()));
		filter->registChild(std::move(unique));
		guids_.emplace_back(std::make_pair(path, guid));
		if (extensions.length())
		{
			XMLDataPtr ext = std::make_shared<XMLData>();
			ext->getElement()->setTag(U"Extensions");
			ext->getElement()->setValue(extensions);
			filter->registChild(std::move(ext));
		}
		filters_->registChild(std::move(filter));
		return true;
	}

	bool EditFilters::addItem(StringView name, FilePathView path, FilePathView filter) noexcept
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
		if (filter.length())
		{
			XMLDataPtr child = std::make_shared<XMLData>();
			child->getElement()->setTag(U"Filter");
			child->getElement()->setValue(Unicode::Widen(filter.toUTF8()));
			item->registChild(std::move(child));
		}
		item_group->registChild(std::move(item));
		return true;
	}

	bool EditFilters::eraseItemGroup(StringView name
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

	bool EditFilters::eraseFilter(FilePathView path) noexcept
	{
		if (filters_ == nullptr)
		{
			Logger << U"フィルターデータが nullptr です。\n";
			return false;
		}
		filters_->eraseChildData([&](XMLData* pData) -> bool
			{
				if (pData->getElement()->getTag().compare(U"Filter") != 0)
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
		for (auto it = guids_.begin(); it != guids_.end(); ++it)
		{
			if (it->first.compare(path) != 0)
			{
				continue;
			}
			guids_.erase(it);
			break;
		}
		return true;
	}

	bool EditFilters::eraseItem(StringView name, FilePathView path) noexcept
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

	XMLData* EditFilters::findFilter(FilePathView path) const noexcept
	{
		if (path.compare(U"") == 0)
		{
			Logger << U"フィルターパスが無効です。\n";
			return nullptr;
		}
		auto findFilterFunc = [&](XMLData* pData) ->bool
		{
			if (pData->getElement()->getTag().compare(U"Filter") != 0)
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
		};
		return filters_->findData(findFilterFunc).get();
	}

	bool EditFilters::existFilter(FilePathView path) const noexcept
	{
		return (findFilter(path) != nullptr);
	}

	bool EditFilters::existGUID(StringView guid) const noexcept
	{
		for (const auto& it : guids_)
		{
			if (it.second.compare(guid) == 0)
			{
				return true;
			}
		}
		return false;
	}

	XMLData* EditFilters::findItemGroup(StringView name) noexcept
	{
		String tag{ name };
		if (tag.compare(U"") == 0)
		{
			tag = U"None";
		}
		for (auto& item_group : item_groups_)
		{
			if(item_group.first.compare(name) != 0)
			{
				continue;
			}
			return item_group.second.get();
		}
		return nullptr;
	}

	void EditFilters::set(XMLInfo& info) noexcept
	{
		filters_ = nullptr;
		item_groups_.clear();
		auto root = info.getRoot();
		if (root == nullptr)
		{
			Logger << U"親データが nullptr です。\n";
			return;
		}
		item_groups_parent_ = root;
		filters_ = root->findData(detail::findFilterItemGroupFunc);
		for (const auto& filter : filters_->getChildren())
		{
			FilterData guid;
			auto attribute = filter.second->getElement()->findAttribute(U"Include");
			if (attribute == nullptr)
			{
				continue;
			}
			guid.first = attribute->getValue();
			auto unique_identifier = filter.second->findElement(U"UniqueIdentifier");
			if (unique_identifier == nullptr)
			{
				continue;
			}
			guid.second = unique_identifier->getValue();
			guids_.emplace_back(guid);
		}
		std::size_t end_point = 0;
		for (auto& child : root->getChildren())
		{
			end_point++;
			if (!detail::isItemGroup(child.second->getElement()))
			{
				continue;
			}
			if (child.second->getChildren().size() == 0)
			{
				continue;
			}
			if (detail::haveUniqueIdentifier(child.second->getChild(0)))
			{
				continue;
			}
			item_groups_.emplace_back(child);
			insert_point_ = end_point - 1;
		}
	}

	XMLData* EditFilters::getFilters() noexcept
	{
		return filters_.get();
	}

	const FilterDatas& EditFilters::getGUIDs() const noexcept
	{
		return guids_;
	}

	const XMLDatas& EditFilters::getItemGroups() const noexcept
	{
		return item_groups_;
	}

	bool EditFilters::existFilterReference(FilePathView path) const noexcept
	{
		//std::size_t count = 0;
		for (const auto& item_group : item_groups_)
		{
			auto filter = item_group.second->findElement(U"Filter");
			if (filter == nullptr)
			{
				continue;
			}
			if (filter->getValue().compare(path) == 0)
			{
				return true;
				//count++;
			}
		}
		return false;
		//return count > 0;
	}
}
