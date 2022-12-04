#include "DialogManager.h"
#include "../ThemeColor/ThemeColorManager.h"
#include "../stdafx.h"

namespace sip
{
	DialogManager::DialogManager() noexcept
		: dialogs_{}
		, result_{ -1 }
		, modal_index_{ 0 }
		, is_skip_frame_{ false }
	{
	}

	DialogManager::~DialogManager() noexcept
	{
		dialogs_.clear();
	}

	void DialogManager::regist(DialogPtr&& dialog) noexcept
	{
		if (dialog->isModal())
		{
			modal_index_ = dialogs_.size();
		}
		dialogs_.emplace_back(std::move(dialog));
		is_skip_frame_ = true;
	}

	bool DialogManager::update() noexcept
	{
		if (is_skip_frame_)
		{
			is_skip_frame_ = false;
			return true;
		}
		const auto dialog_count = dialogs_.size();
		for (std::size_t i = modal_index_; i < dialog_count; i++)
		{
			if (dialogs_[i]->update())
			{
				continue;
			}
			result_ = dialogs_[i]->result();
			bool is_modal = dialogs_[i]->isModal();
			dialogs_.pop_back();
			if (is_modal)
			{
				refreshModalIndex();
			}
			return false;
		}
		return true;
	}

	void DialogManager::draw() const noexcept
	{
		if (!existDialog())
		{
			return;
		}
		auto col_mng = g_pGetBlackboard(ThemeColorManager* const)->get("theme_color_manager");
		for (const auto& dialog : dialogs_)
		{
			if (dialog->isEnableCover())
			{
				const auto& cover_color = col_mng->getDialogCover();
				Rect{ 0, 0, Scene::Size() }.draw(cover_color);
			}
			dialog->draw();
		}
	}

	bool DialogManager::existDialog() const noexcept
	{
		return (modal_index_ < dialogs_.size());
	}

	std::int32_t DialogManager::result() const noexcept
	{
		return result_;
	}

	void DialogManager::refreshModalIndex() noexcept
	{
		std::size_t tmp_index = 0;
		for (const auto& it : dialogs_)
		{
			if (it->isModal())
			{
				modal_index_ = tmp_index;
			}
			tmp_index++;
		}
	}
}
