#include "SimpleComboBox.h"
#include "../stdafx.h"

namespace sip
{
	SimpleComboBox::SimpleComboBox(const Array<String>* items, Font* font, const Vec2& pos, const SizeF& size)
		: font_{ font }
		, items_{ items }
		, index_{ 0 }
		, padding_{ 6, 2 }
		, scroll_{ 0.0, 0.0 }
		, scroll_max_{ 0.0, padding_.y }
		, rect_{ pos, size.x, ((*font).height() + padding_.y * 2.0) }
		, pulldown_rect_{ rect_.bl(), size }
		, down_button_size_{ 16 }
		, is_open_{ false }
	{
		for (const auto& item : *items_)
		{
			scroll_max_.x = Max(scroll_max_.x, (*font_)(item).region().w + padding_.x * 2);
			scroll_max_.y += (*font).height() + padding_.y;
		}
		if (size.x == 0.0)
		{
			rect_.w = scroll_max_.x + down_button_size_;
			pulldown_rect_.w = rect_.w;
		}
		if (size.y == 0.0)
		{
			pulldown_rect_.h = scroll_max_.y;
		}
		RenderTexture tmp{ static_cast<uint32>(pulldown_rect_.w), static_cast<uint32>(pulldown_rect_.h) };
		pulldown_render_target_.swap(tmp);
	}

	bool SimpleComboBox::isEmpty() const noexcept
	{
		return items_->isEmpty();
	}

	void SimpleComboBox::update()
	{
		if (isEmpty())
		{
			return;
		}

		if (rect_.leftClicked())
		{
			is_open_ = (not is_open_);
		}


		if (is_open_)
		{
			if (pulldown_rect_.mouseOver())
			{
				scroll_max_.y = padding_.y;
				for (const auto& item : *items_)
				{
					scroll_max_.x = Max(scroll_max_.x, (*font_)(item).region().w + padding_.x * 2);
					scroll_max_.y += (*font_).height() + padding_.y;
				}
				if (KeyShift.pressed())
				{
					scroll_.x += Mouse::Wheel() * 8;
				}
				else
				{
					scroll_.y += Mouse::Wheel() * 8;
				}
				Vec2 pos = rect_.pos.movedBy(-scroll_.x, rect_.h - scroll_.y);
				for (auto i : step(items_->size()))
				{
					const RectF rect{ pos, rect_.w, rect_.h };
					if (rect.leftClicked())
					{
						Mouse::ClearLRInput();
						index_ = i;
						is_open_ = false;
						break;
					}

					pos.y += rect_.h;
				}
			}
			scroll_.y = Clamp(scroll_.y, 0.0, scroll_max_.y - pulldown_rect_.h + (padding_.y * (items_->size() + 1)));
			scroll_.x = Clamp(scroll_.x, 0.0, scroll_max_.x - pulldown_rect_.w + (padding_.x * 2));
		}
	}

	void SimpleComboBox::draw() const
	{
		rect_.rounded(5)
			.drawShadow({ -2, -2 }, 5.0, 0.0, Palette::Whitesmoke)
			.drawShadow({  2,  2 }, 5.0, 0.0, Palette::Darkgray)
			.draw(Palette::Lightgray);

		if (isEmpty())
		{
			return;
		}

		//rect_.drawFrame(1, 0, is_open_ ? Palette::Orange : Palette::Gray);

		Vec2 pos = rect_.pos;

		(*font_)((*items_)[index_]).draw(
			RectF{
				pos + padding_,
				rect_.size - SizeF{ down_button_size_ + padding_.x, 0.0 }
			},
			Palette::Dimgray);

		Triangle{ (rect_.x + rect_.w - down_button_size_ / 2.0 - padding_.x), (rect_.y + rect_.h / 2.0),
			(down_button_size_ * 0.5), 180_deg }.draw(Palette::Dimgray);

		pos.y += rect_.h;

		if (is_open_)
		{
			pulldown_render_target_.clear(Palette::Lightgray);
			{
				double offset_y = -scroll_.y;
				ScopedRenderTarget2D target{ pulldown_render_target_ };
				for (const auto& item : *items_)
				{
					const RectF rect{ 0, offset_y, rect_.size };
					if (rect.movedBy(pulldown_rect_.pos).mouseOver())
					{
						rect.draw(Palette::Skyblue);
					}

					(*font_)(item).draw((Vec2{ -scroll_.x, offset_y } + padding_), Palette::Dimgray);

					offset_y += rect_.h;
				}
			}

			pulldown_render_target_.region().movedBy(pulldown_rect_.asRect().pos)
				.rounded(5)
				.drawShadow({  2,  2 }, 5.0, 0.0, Palette::Whitesmoke)
				.drawShadow({ -2, -2 }, 5.0, 0.0, Palette::Darkgray)
				.draw(Palette::Lightgray);
			pulldown_render_target_.draw(pulldown_rect_.pos);
		}
	}

	void SimpleComboBox::setPos(const Vec2& pos) noexcept
	{
		rect_.setPos(pos);
	}

	void SimpleComboBox::setIndex(size_t index) noexcept
	{
		index_ = index;
	}

	const RectF& SimpleComboBox::getRect() const noexcept
	{
		return rect_;
	}

	size_t SimpleComboBox::getSelectIndex() const noexcept
	{
		return index_;
	}

	String SimpleComboBox::getSelectItem() const noexcept
	{
		if (isEmpty())
		{
			return U"";
		}
		return items_->at(index_);
	}

	void SimpleComboBox::close() noexcept
	{
		is_open_ = false;
	}

	bool SimpleComboBox::isOpen() const noexcept
	{
		return is_open_;
	}
}
