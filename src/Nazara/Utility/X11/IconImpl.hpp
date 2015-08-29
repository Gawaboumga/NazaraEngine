// Copyright (C) 2015 Jérôme Leclercq
// This file is part of the "Nazara Engine - Utility module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_ICONIMPL_HPP
#define NAZARA_ICONIMPL_HPP

#include <xcb/xcb.h>

class NzImage;

class NzIconImpl
{
	public:
		bool Create(const NzImage& image);
		void Destroy();

		xcb_pixmap_t GetIcon();

	private:
		xcb_pixmap_t m_icon;
};

#endif // NAZARA_ICONIMPL_HPP
