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

		/// @brief 
		/// @return 
		[[nodiscard]] static bool eraseResource() noexcept;
	};

	/// @brief 
	class MenuEnableFunc
	{
	private:

		explicit MenuEnableFunc() = delete;
		explicit MenuEnableFunc(const MenuEnableFunc&) = delete;
		explicit MenuEnableFunc(MenuEnableFunc&&) = delete;
		virtual ~MenuEnableFunc() = delete;

	public:

		/// @brief 
		/// @return 
		[[nodiscard]] static bool isOpen() noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] static bool existExecCommand() noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] static bool existUndoCommand() noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]] static bool existSelectUserResource() noexcept;

	};
}

#endif // !SIP_MENU_FUNC_H_
