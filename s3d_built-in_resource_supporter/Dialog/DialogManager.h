#pragma once
#ifndef SIP_DIALOG_MANAGER_H_
#define SIP_DIALOG_MANAGER_H_

#include "IDialog.h"
#include <vector>

namespace sip
{
	/// @brief 
	class DialogManager
	{
	public:

		/// @brief 
		explicit DialogManager() noexcept;

		/// @brief 
		~DialogManager() noexcept;

		/// @brief 
		/// @param dialog 
		void regist(DialogPtr&& dialog) noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] bool update() noexcept;

		/// @brief 
		/// @return 
		void draw() /* const */ noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] bool existDialog() const noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] std::int32_t result() const noexcept;

	private:

		/// @brief 
		void refreshModalIndex() noexcept;

	private:

		/// @brief 
		std::vector<DialogPtr> dialogs_;

		/// @brief 
		std::int32_t result_;

		/// @brief 
		std::size_t modal_index_;

		/// @brief 
		bool is_skip_frame_;
	};
}


#endif // !SIP_DIALOG_MANAGER_H_
