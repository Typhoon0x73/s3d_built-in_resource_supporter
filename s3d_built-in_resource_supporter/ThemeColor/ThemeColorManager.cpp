#include "ThemeColorManager.h"
#include "../stdafx.h"
#include "ThemeColorLight.h"

namespace sip
{
	class ThemeFactory
	{
	private:
		explicit ThemeFactory() = delete;
		~ThemeFactory() = delete;

	public:
		static ThemeColorBase* Create(ThemeColor theme)
		{
			switch (theme)
			{
			case ThemeColor::Light: return new ThemeColorBase(theme_color_light);
			case ThemeColor::Dark: return nullptr;
			default: return nullptr;
			}
		}
	};

	ThemeColorManager::ThemeColorManager() noexcept
		: theme_color_{ nullptr }
	{
	}

	ThemeColorManager::~ThemeColorManager() noexcept
	{
	}

	void ThemeColorManager::SetTheme(ThemeColor theme) noexcept
	{
		theme_color_.reset(ThemeFactory::Create(theme));
	}

	const ColorF& ThemeColorManager::getMainBackground() const noexcept
	{
		return theme_color_->getColor(ColorTag::MainBackground);
	}

	const ColorF& ThemeColorManager::getDialogCover() const noexcept
	{
		return theme_color_->getColor(ColorTag::DialogCover);
	}
}
