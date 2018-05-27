// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Platform module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_CURSORIMPL_HPP
#define NAZARA_CURSORIMPL_HPP

#include <Nazara/Prerequisites.hpp>
#include <Nazara/Platform/Enums.hpp>
#include <array>

struct SDL_Cursor;
struct SDL_Surface;

namespace Nz
{
	class Image;

	class CursorImpl
	{
		friend class Cursor;

		public:
			bool Create(const Image& image, int hotSpotX, int hotSpotY);
			bool Create(SystemCursor cursor);

			void Destroy();

			SDL_Cursor* GetCursor();

		private:
			SDL_Cursor* m_cursor = nullptr;
			SDL_Surface* m_surface = nullptr;
	};
}

#endif // NAZARA_CURSORIMPL_HPP
