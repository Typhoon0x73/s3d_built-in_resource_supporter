#include "EraseResourceDialog.h"
#include "../stdafx.h"

namespace sip
{
	EraseResourceDialog::EraseResourceDialog()
		: result_{ 0 }
	{
	}

	EraseResourceDialog::~EraseResourceDialog()
	{
	}

	bool EraseResourceDialog::update()
	{
		return false;
	}

	void EraseResourceDialog::draw() const
	{
	}

	bool EraseResourceDialog::isModal() const
	{
		return true;
	}

	bool EraseResourceDialog::isEnableCover() const
	{
		return true;
	}

	std::int32_t EraseResourceDialog::result() const
	{
		return result_;
	}
}
