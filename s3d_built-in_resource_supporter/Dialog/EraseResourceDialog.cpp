#include "EraseResourceDialog.h"
#include "../stdafx.h"

namespace sip
{
	EraseResourceDialog::EraseResourceDialog() noexcept
	{
	}

	EraseResourceDialog::~EraseResourceDialog() noexcept
	{
	}

	bool EraseResourceDialog::update() noexcept
	{
		return false;
	}

	void EraseResourceDialog::draw() const noexcept
	{
	}

	bool EraseResourceDialog::isModal() const noexcept
	{
		return true;
	}

	bool EraseResourceDialog::isEnableCover() const noexcept
	{
		return true;
	}

	std::int32_t EraseResourceDialog::result() const noexcept
	{
		return result_;
	}
}
