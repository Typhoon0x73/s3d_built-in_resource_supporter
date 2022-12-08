#pragma once
#ifndef SIP_ERASE_RESOURCE_DIALOG_H_
#define SIP_ERASE_RESOURCE_DIALOG_H_

#include "IDialog.h"

namespace sip
{
	class EraseResourceDialog : public IDialog
	{
	public:

		enum class Result : std::int32_t
		{
			Failed = -1,
			None,
			Cancel,
			Success,
		};

	public:

		/// @brief 
		explicit EraseResourceDialog() noexcept;

		/// @brief 
		virtual ~EraseResourceDialog() noexcept;

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
		Font* font_;

		/// @brief 
		RectF base_rect_;

		/// @brief 
		RectF cancel_rect_;

		/// @brief 
		RectF erase_rect_;

		/// @brief 
		Result result_;

		/// @brief 
		Texture cancel_texture_;

		/// @brief 
		Texture erase_texture_;
	};
}

#endif // !SIP_ERASE_RESOURCE_DIALOG_H_
