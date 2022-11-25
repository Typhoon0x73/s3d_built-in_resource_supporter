#include "SimpleComboBox.h"
#include "../stdafx.h"

namespace sip
{
	SimpleComboBox::SimpleComboBox(const Array<String>& items, const Font& font, const Vec2& pos)
		: font_{ font }
		, items_{ items }
		, index_{ 0 }
		, padding_{ 6, 2 }
		, rect_{ pos, 0, (font.height() + padding_.y * 2.0) }
		, down_button_size_{ 16 }
		, is_open_{ false }
	{
		for (const auto& item : items_)
		{
			rect_.w = Max(rect_.w, font_(item).region().w);
		}
		rect_.w += (padding_.x * 2 + down_button_size_);
	}

	bool SimpleComboBox::isEmpty() const noexcept
	{
		return items_.isEmpty();
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

		Vec2 pos = rect_.pos.movedBy(0, rect_.h);

		if (is_open_)
		{
			for (auto i : step(items_.size()))
			{
				if (const RectF rect{ pos, rect_.w, rect_.h };
					rect.leftClicked())
				{
					Mouse::ClearLRInput();
					index_ = i;
					is_open_ = false;
					break;
				}

				pos.y += rect_.h;
			}
		}
	}

	void SimpleComboBox::draw() const
	{
		rect_.draw();

		if (isEmpty())
		{
			return;
		}

		rect_.drawFrame(1, 0, is_open_ ? Palette::Orange : Palette::Gray);

		Vec2 pos = rect_.pos;

		font_(items_[index_]).draw(pos + padding_, Palette::Black);

		Triangle{ (rect_.x + rect_.w - down_button_size_ / 2.0 - padding_.x), (rect_.y + rect_.h / 2.0),
			(down_button_size_ * 0.5), 180_deg }.draw(Palette::Black);

		pos.y += rect_.h;

		if (is_open_)
		{
			const RectF backRect{ pos, rect_.w, (rect_.h * items_.size()) };

			backRect.drawShadow({ 1, 1 }, 4, 1).draw();

			for (const auto& item : items_)
			{
				if (const RectF rect{ pos, rect_.size };
					rect.mouseOver())
				{
					rect.draw(Palette::Skyblue);
				}

				font_(item).draw((pos + padding_), Palette::Black);

				pos.y += rect_.h;
			}

			backRect.drawFrame(1, 0, Palette::Gray);
		}
	}

	void SimpleComboBox::setPos(const Vec2& pos) noexcept
	{
		rect_.setPos(pos);
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
		return items_[index_];
	}
}
