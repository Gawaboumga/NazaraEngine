// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Platform module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Platform/SDL2/VideoModeImpl.hpp>
#include <Nazara/Core/Error.hpp>
#include <Nazara/Core/String.hpp>
#include <Nazara/Platform/VideoMode.hpp>
#include <algorithm>
#include <SDL2/SDL.h>
#include <Nazara/Platform/Debug.hpp>

namespace Nz
{
	VideoMode VideoModeImpl::GetDesktopMode()
	{
		SDL_DisplayMode mode;
		if (SDL_GetDesktopDisplayMode(0, &mode) != 0)
		{
			NazaraError(String("Unable to retrieve current display mode") + SDL_GetError());
			return VideoMode();
		}
		return VideoMode(mode.w, mode.h, SDL_BITSPERPIXEL(mode.format));
	}

	void VideoModeImpl::GetFullscreenModes(std::vector<VideoMode>& modes)
	{
		int displayVideoCount = SDL_GetNumVideoDisplays();
		if (displayVideoCount < 1)
		{
			NazaraError("Unable to retrieve video display");
			return;
		}

		for (int displayIndex = 0; displayIndex != displayVideoCount; ++displayIndex)
		{
			int displayModeCount = SDL_GetNumDisplayModes(displayIndex);
			if (displayModeCount < 1)
			{
				NazaraError("Unable to retrieve video mode for display: " + String::Number(displayIndex));
				continue;
			}

			for (int modeIndex = 0; modeIndex != displayModeCount; ++modeIndex)
			{
				SDL_DisplayMode retrievedMode;
				SDL_GetDisplayMode(displayIndex, modeIndex, &retrievedMode);

				VideoMode mode(retrievedMode.w, retrievedMode.h, SDL_BITSPERPIXEL(retrievedMode.format));

				// Il existe plusieurs modes avec ces trois caractéristques identiques
				if (std::find(modes.begin(), modes.end(), mode) == modes.end())
					modes.push_back(mode);
			}
		}
	}
}
