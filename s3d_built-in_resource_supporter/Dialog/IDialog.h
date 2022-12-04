#pragma once
#ifndef SIP_IDIALOG_H_
#define SIP_IDIALOG_H_

#include <memory>

namespace sip
{
	/// @brief 
	class IDialog
	{
	public:

		/// @brief 
		virtual ~IDialog() noexcept = default;

		/// @brief 
		/// @return 
		[[nodiscard]] virtual bool update() noexcept = 0;

		/// @brief 
		virtual void draw() const noexcept = 0;

		/// @brief
		/// @return 
		[[nodiscard]] virtual bool isModal() const noexcept = 0;

		/// @brief 
		/// @return 
		[[nodiscard]] virtual bool isEnableCover() const noexcept = 0;

		/// @brief 
		/// @return 
		[[nodiscard]] virtual std::int32_t result() const noexcept = 0;
	};

	/// @brief 
	using DialogPtr = std::unique_ptr<IDialog>;
}

#endif // !SIP_IDIALOG_H_
