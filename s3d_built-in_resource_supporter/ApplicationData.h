#pragma once
#ifndef SIP_APPLICATION_DATA_H_
#define SIP_APPLICATION_DATA_H_

#include <Siv3D.hpp>
#include "ToolDefine.h"

namespace sip
{
	class XMLInfo;
	class EditVcxproj;
	class EditFilters;
	class ResourceInfo;
	class CommandManager;
	class ThemeColorManager;
	class DialogManager;
	struct WindowInfo;

	/// @brief 
	class ApplicationData
	{
	public:

		/// @brief 
		explicit ApplicationData() noexcept;

		/// @brief 
		~ApplicationData() noexcept;

	private:

		/// @brief 
		std::unique_ptr<WindowInfo> window_info_;

		/// @brief 
		std::unique_ptr<CommandManager> command_manager_;

		/// @brief 
		std::unique_ptr<ThemeColorManager> theme_color_manager_;

		/// @brief 
		std::unique_ptr<DialogManager> dialog_manager_;

		/// @brief 
		std::unique_ptr<XMLInfo> vcxproj_info_;

		/// @brief 
		std::unique_ptr<XMLInfo> filters_info_;

		/// @brief 
		std::unique_ptr<EditVcxproj> edit_vcxproj_;

		/// @brief 
		std::unique_ptr<EditFilters> edit_filters_;

		/// @brief 
		std::unique_ptr<ResourceInfo> resource_info_;

		/// @brief 
		std::unique_ptr<FilePath> relative_path_;

		/// @brief 
		std::unique_ptr<FilePath> vcxproj_path_;

		/// @brief 
		std::unique_ptr<FilePath> resource_path_;

		/// @brief 
		std::unique_ptr<TagParams> tag_page_list_params_;

		/// @brief 
		std::unique_ptr<ResourceParams> resource_page_list_params_;
	};
}

#endif // !SIP_APPLICATION_DATA_H_

