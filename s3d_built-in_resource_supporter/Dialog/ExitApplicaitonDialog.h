#pragma once
#ifndef SIP_EXIT_APPLICATION_DIALOG_H_
#define SIP_EXIT_APPLICATION_DIALOG_H_

#include "IDialog.h"

namespace sip
{
	class ExitApplicationDialog : public IDialog
	{
	public:

		enum class Result : std::int32_t
		{
			Cancel = -1,
			None,
			Ok,
		};

	public:

		/// @brief 
		explicit ExitApplicationDialog() noexcept;

		/// @brief 
		virtual ~ExitApplicationDialog() noexcept override;

		/// @brief 
		/// @return 
		[[nodiscard]] virtual bool update() noexcept override;

		/// @brief 
		virtual void draw() const noexcept override;

		/// @brief 
		/// @return 
		[[nodiscard]] virtual bool isModal() const noexcept override;

		/// @brief 
		/// @return 
		[[nodiscard]] virtual bool isEnableCover() const noexcept override;

		/// @brief 
		/// @return 
		[[nodiscard]] virtual std::int32_t result() const noexcept override;

	private:

		/// @brief 
		RectF base_rect_;

		/// @brief 
		RectF cancel_rect_;

		/// @brief 
		RectF ok_rect_;

		/// @brief 
		Texture cancel_texture_;

		/// @brief 
		Texture ok_texture_;

		/// @brief 
		Result result_;

		/// @brief 
		Font* font_;
	};
}

#endif // !SIP_EXIT_APPLICATION_DIALOG_H_
