#include "../stdafx.h"
#include "ResourceView.h"

namespace sip
{
	ResourceView::ResourceView(const RectF& rect, size_t* tab_no) noexcept
		: render_rect_{ rect }
		, toggle_render_rect_{ render_rect_.stretched(0, -300, 0, -10) }
		, resource_render_target_{}
	{
	}

	ResourceView::~ResourceView() noexcept
	{
	}

	void ResourceView::update() noexcept
	{
	}

	void ResourceView::draw() const noexcept
	{
	}
}
