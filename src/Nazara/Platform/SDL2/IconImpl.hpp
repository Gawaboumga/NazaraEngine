// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Platform module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_ICONIMPL_HPP
#define NAZARA_ICONIMPL_HPP

#include <Nazara/Prerequisites.hpp>

struct SDL_Surface;

namespace Nz
{
	class Image;

	class IconImpl
	{
		public:
			bool Create(const Image& image);
			void Destroy();

			SDL_Surface* GetIcon();

		private:
			SDL_Surface* m_icon = nullptr;
	};
}

#endif // NAZARA_ICONIMPL_HPP
