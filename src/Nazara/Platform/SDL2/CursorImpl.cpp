// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Platform module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Platform/SDL2/CursorImpl.hpp>
#include <Nazara/Core/Error.hpp>
#include <Nazara/Utility/Image.hpp>
#include <Nazara/Utility/PixelFormat.hpp>
#include <SDL2/SDL.h>
#include <Nazara/Platform/Debug.hpp>

namespace Nz
{
	bool CursorImpl::Create(const Image& cursor, int hotSpotX, int hotSpotY)
	{
		Image windowsCursor(cursor);
		if (!windowsCursor.Convert(PixelFormatType_BGRA8))
		{
			NazaraError("Failed to convert cursor to BGRA8");
			return false;
		}

		SDL_Surface* cursorSurface = SDL_CreateRGBSurfaceWithFormatFrom(
			static_cast<void*>(windowsCursor.GetPixels()), // Data
			windowsCursor.GetWidth(), windowsCursor.GetHeight(), windowsCursor.GetDepth(),
			windowsCursor.GetBytesPerPixel() * windowsCursor.GetWidth(), // Pitch: the length of a row of pixels in bytes
			SDL_PIXELFORMAT_BGRA8888
		);

		if (!cursorSurface)
		{
			NazaraError(String("Unable to create surface from the given image: ") + SDL_GetError());
			return false;
		}

		SDL_Cursor* sdlCursor = SDL_CreateColorCursor(cursorSurface, hotSpotX, hotSpotY);
		if (!sdlCursor)
		{
			NazaraError(String("Unable to create cursor: ") + SDL_GetError());
			SDL_FreeSurface(cursorSurface);
			return false;
		}

		m_cursor = sdlCursor;
		m_surface = cursorSurface;

		return true;
	}

	bool CursorImpl::Create(SystemCursor cursor)
	{
		SDL_Cursor* sdlCursor;
		switch (cursor)
		{
			case SystemCursor_Crosshair:
				sdlCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
				break;
			case SystemCursor_Default:
				sdlCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
				break;
			case SystemCursor_Hand:
				sdlCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
				break;
			case SystemCursor_None:
				sdlCursor = nullptr;
				break;
			case SystemCursor_Pointer:
				sdlCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
				break;
			case SystemCursor_Progress:
				sdlCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAIT);
				break;
			case SystemCursor_Resize:
				sdlCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
				break;
			case SystemCursor_ResizeNESW:
				sdlCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
				break;
			case SystemCursor_ResizeNS:
				sdlCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
				break;
			case SystemCursor_ResizeNWSE:
				sdlCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
				break;
			case SystemCursor_ResizeWE:
				sdlCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
				break;
			case SystemCursor_Text:
				sdlCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
				break;
			case SystemCursor_Wait:
				sdlCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAIT);
				break;
			default:
				NazaraError("SystemCursor not handled (0x" + String::Number(cursor, 16) + ')');
				sdlCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
				break;
		}
		m_cursor = sdlCursor;

		return true;
	}

	void CursorImpl::Destroy()
	{
		if (m_surface)
			SDL_FreeSurface(m_surface);
		if (m_cursor)
			SDL_FreeCursor(m_cursor);
	}

	SDL_Cursor* CursorImpl::GetCursor()
	{
		return m_cursor;
	}
}
