#include "SettingsINI.h"
#include "WindowInfo.h"

namespace sip
{
	Point GetWindowPosFromINI(const INI& ini)
	{
		const auto& window_pos_section = ini.getSection(U"window_pos");
		int32 x = -1;
		int32 y = -1;
		if (window_pos_section.keys.size() < 2)
		{
			return Point{ x, y };
		}
		for (const auto& it : window_pos_section.keys)
		{
			if (it.name.compare(U"x") == 0)
			{
				x = ParseOr<int32>(it.value, 0);
			}
			if (it.name.compare(U"y") == 0)
			{
				y = ParseOr<int32>(it.value, 0);
			}
		}
		return Point{ x, y };
	}

	Size GetWindowSizeFromINI(const INI& ini)
	{
		const auto& window_size_section = ini.getSection(U"window_size");
		const auto& window_info = g_pGetBlackboard(WindowInfo* const)->get("window_info");
		size_t scene_width  = window_info->default_size_.x;
		size_t scene_height = window_info->default_size_.y;
		if (window_size_section.keys.size() < 2)
		{
			return Size{ scene_width, scene_height };
		}
		for (const auto& it : window_size_section.keys)
		{
			if (it.name.compare(U"width") == 0)
			{
				scene_width = ParseOr<size_t>(it.value, scene_width);
			}
			if (it.name.compare(U"height") == 0)
			{
				scene_height = ParseOr<size_t>(it.value, scene_height);
			}
		}
		return Size{ scene_width, scene_height };
	}

	bool GetWindowMaximizedFromINI(const INI& ini)
	{
		const auto& window_state_section = ini.getSection(U"window_state");
		if (window_state_section.keys.size() <= 0)
		{
			return false;
		}
		for (const auto& it : window_state_section.keys)
		{
			if (it.name.compare(U"maximized") == 0)
			{
				return (it.value.compare(U"true") == 0);
			}
		}
		return false;
	}

	bool ApplySettings(const INI& ini)
	{
		Window::Resize(GetWindowSizeFromINI(ini));
		auto pos = GetWindowPosFromINI(ini);
		if (pos.x >= 0 and pos.y >= 0)
		{
			Window::SetPos(pos);
		}
		if (GetWindowMaximizedFromINI(ini))
		{
			Window::Maximize();
		}
		return true;
	}

	bool LoadSettingsINI(FilePathView path)
	{
		INI ini;
		if (not ini.load(path))
		{
			return false;
		}
		return ApplySettings(ini);
	}

	bool SaveSettingsINI(FilePathView path)
	{
		INI ini;
		ini.addSection(U"window_size");
		ini.addSection(U"window_pos");
		ini.addSection(U"window_state");
		ini.write(U"window_size", U"width", Scene::Width());
		ini.write(U"window_size", U"height", Scene::Height());
		ini.write(U"window_pos", U"x", Window::GetPos().x);
		ini.write(U"window_pos", U"y", Window::GetPos().y);
		ini.write(U"window_state", U"maximized", Window::GetState().maximized);
		if (not ini.save(path))
		{
			return false;
		}
		return true;
	}
}

