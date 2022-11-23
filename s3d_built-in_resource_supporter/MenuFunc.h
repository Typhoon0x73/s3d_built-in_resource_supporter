#pragma once
#ifndef SIP_MENU_FUNC_H_
#define SIP_MENU_FUNC_H_

#include <Siv3D.hpp>

namespace sip
{
	/// @brief 
	class MenuFunc
	{
	private:

		explicit MenuFunc() = delete;
		explicit MenuFunc(const MenuFunc&) = delete;
		explicit MenuFunc(MenuFunc&&) = delete;
		virtual ~MenuFunc() = delete;

	public:

		/// @brief 
		/// @return 
		[[nodiscard]] static bool fileOpen() noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] static bool fileSave() noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] static bool fileClose() noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] static bool undo() noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] static bool redo() noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] static bool registResource() noexcept;
	};
}

#endif // !SIP_MENU_FUNC_H_
