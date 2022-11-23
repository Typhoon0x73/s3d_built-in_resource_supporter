#pragma once
#ifndef SIP_SETTINGS_INI_H_
#define SIP_SETTINGS_INI_H_
namespace sip
{
	/// @brief 設定の適応
	/// @param ini INIデータ
	/// @return true : 成功, false : 失敗
	bool ApplySettings(const INI& ini);

	/// @brief
	/// @param path 
	/// @return true : 成功, false : 失敗
	bool LoadSettingsINI(FilePathView path);

	/// @brief 
	/// @param path 
	/// @return true : 成功, false : 失敗
	bool SaveSettingsINI(FilePathView path);
}
#endif // !SIP_SETTINGS_INI_H_
