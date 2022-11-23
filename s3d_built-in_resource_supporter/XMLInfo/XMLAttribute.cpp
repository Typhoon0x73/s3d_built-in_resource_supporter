#include "XMLAttribute.h"

namespace sip
{
	XMLAttribute::XMLAttribute() noexcept
		: name_{ U"" }
		, value_{ U"" }
	{
	}

	XMLAttribute::XMLAttribute(StringView name, StringView value) noexcept
		: name_{ name }
		, value_{ value }
	{
	}

	XMLAttribute::~XMLAttribute() noexcept
	{
	}

	const String& XMLAttribute::getName() const noexcept
	{
		return name_;
	}

	const String& XMLAttribute::getValue() const noexcept
	{
		return value_;
	}

	void XMLAttribute::setName(StringView name) noexcept
	{
		name_ = name;
	}

	void XMLAttribute::setValue(StringView value) noexcept
	{
		value_ = value;
	}

	String XMLAttribute::format() const noexcept
	{
		return U"{}=\"{}\""_fmt(name_, value_);
	}
}

