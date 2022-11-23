#include "XMLElement.h"
#include "XMLAttribute.h"

namespace sip
{
	XMLElement::XMLElement() noexcept
		: tag_{ U"" }
		, value_{ U"" }
		, attributes_{}
	{
	}

	XMLElement::~XMLElement() noexcept
	{
		clear();
	}

	void XMLElement::clear() noexcept
	{
		tag_ = U"";
		value_ = U"";
		attributes_.clear();
	}

	void XMLElement::registAttribute(StringView name, StringView value) noexcept
	{
		attributes_.emplace_back(new XMLAttribute(name, value));
	}

	XMLAttribute* XMLElement::findAttribute(StringView name) noexcept
	{
		for (auto& attribute : attributes_)
		{
			if (attribute->getName().compare(name) != 0)
			{
				continue;
			}
			return attribute.get();
		}
		return nullptr;
	}

	XMLAttribute* XMLElement::getAttribute(std::size_t index) noexcept
	{
		if (index >= attributes_.size())
		{
			return nullptr;
		}
		return attributes_[index].get();
	}

	const XMLAttributes& XMLElement::getAttributes() const noexcept
	{
		return attributes_;
	}

	const String& XMLElement::getTag() const noexcept
	{
		return tag_;
	}

	const String& XMLElement::getValue() const noexcept
	{
		return value_;
	}

	void XMLElement::setTag(StringView tag) noexcept
	{
		tag_ = tag;
	}

	void XMLElement::setValue(StringView value) noexcept
	{
		value_ = value;
	}
}
