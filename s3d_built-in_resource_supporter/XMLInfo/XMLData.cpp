#include "XMLData.h"
#include "XMLAttribute.h"
#include "XMLElement.h"

namespace sip
{
	XMLData::XMLData() noexcept
		: element_{ std::make_shared<XMLElement>() }
		, children_{}
	{
	}

	XMLData::~XMLData() noexcept
	{
		clear();
	}

	void XMLData::clear() noexcept
	{
		element_->clear();
		children_.clear();
	}

	void XMLData::registChild(XMLDataPtr&& data) noexcept
	{
		String tag = U"";
		if (data->getChildren().size() > 0)
		{
			tag = data->getChild(0)->getElement()->getTag();
		}
		children_.emplace_back(std::make_pair(tag, std::move(data)));
	}

	void XMLData::insertChild(StringView name, XMLDataPtr data, std::size_t index) noexcept
	{
		children_.insert(children_.begin() + index, std::make_pair(String(name), data));
	}

	void XMLData::eraseChildData(const XMLDataFindFunc& func, std::size_t* erase_index) noexcept
	{
		std::size_t index = 0;
		for (auto it = children_.begin(); it != children_.end(); ++it, index++)
		{
			if (!func(it->second.get()))
			{
				continue;
			}
			if (erase_index)
			{
				(*erase_index) = index;
			}
			children_.erase(it);
			break;
		}
	}

	void XMLData::eraseChildData(StringView name, std::size_t* erase_index) noexcept
	{
		std::size_t index = 0;
		for (auto it = children_.begin(); it != children_.end(); ++it, index++)
		{
			if (it->first.compare(name) != 0)
			{
				continue;
			}
			if (erase_index)
			{
				(*erase_index) = index;
			}
			children_.erase(it);
			break;
		}
	}

	XMLDataPtr XMLData::findData(StringView tag) noexcept
	{
		auto func = [&](XMLData* pData)
		{
			if (pData == nullptr)
			{
				return false;
			}
			return (
				pData->getElement()
				->getTag()
				.compare(tag) == 0
				);
		};
		return findData(func);
	}

	XMLDataPtr XMLData::findData(const XMLDataFindFunc& func) noexcept
	{
		if (func(this))
		{
			return shared_from_this();
		}

		for (auto& child : children_)
		{
			auto data = child.second->findData(func);
			if (data == nullptr)
			{
				continue;
			}
			return data;
		}
		return nullptr;
	}

	XMLElement* XMLData::findElement(StringView tag) noexcept
	{
		if (element_ != nullptr)
		{
			if (element_->getTag().compare(tag) == 0)
			{
				return element_.get();
			}
		}

		for (auto& child : children_)
		{
			XMLElement* element = child.second->findElement(tag);
			if (element == nullptr)
			{
				continue;
			}
			return element;
		}
		return nullptr;
	}

	XMLElement* XMLData::getElement() noexcept
	{
		return element_.get();
	}

	const XMLElement* XMLData::getElement() const noexcept
	{
		return element_.get();
	}

	XMLData* XMLData::getChild(std::size_t index) noexcept
	{
		if (index >= children_.size())
		{
			return nullptr;
		}
		return children_[index].second.get();
	}

	const XMLDatas& XMLData::getChildren() const noexcept
	{
		return children_;
	}

	bool XMLData::existData() const noexcept
	{
		return (element_->getAttributes().size() > 0)
			&& (element_->getTag().compare(U"") != 0)
			&& (element_->getValue().compare(U"") != 0)
			&& (children_.size() > 0)
			;
	}
}
