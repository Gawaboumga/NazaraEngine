// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Platform module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Platform/SDL2/IconImpl.hpp>
#include <Nazara/Utility/Image.hpp>
#include <Nazara/Utility/PixelFormat.hpp>
#include <SDL2/SDL.h>
#include <Nazara/Platform/Debug.hpp>

namespace Nz
{
	bool IconImpl::Create(const Image& icon)
	{
		if (icon.GetHeight() > 256 || icon.GetWidth() > 256)
		{
			NazaraError("It is unlikely that the window manager handle these icon dimensions");
			return false;
		}

		Image windowsIcon(icon);
		if (!windowsIcon.Convert(PixelFormatType_RGBA8))
		{
			NazaraError("Failed to convert icon to RGBA8");
			return false;
		}

		SDL_Surface* iconSurface = SDL_CreateRGBSurfaceWithFormatFrom(
			static_cast<void*>(windowsIcon.GetPixels()), // Data
			windowsIcon.GetWidth(), windowsIcon.GetHeight(), windowsIcon.GetDepth(),
			windowsIcon.GetBytesPerPixel() * windowsIcon.GetWidth(), // Pitch: the length of a row of pixels in bytes
			SDL_PIXELFORMAT_RGBA8888
		);

		if (!iconSurface)
		{
			NazaraError(String("Unable to create surface from the given image: ") + SDL_GetError());
			return false;
		}

		m_icon = iconSurface;

		return true;
	}

	void IconImpl::Destroy()
	{
		if (m_icon)
			SDL_FreeSurface(m_icon);
	}

	SDL_Surface* IconImpl::GetIcon()
	{
		return m_icon;
	}
}
