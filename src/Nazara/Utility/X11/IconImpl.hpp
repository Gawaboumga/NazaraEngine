// Copyright (C) 2015 Jérôme Leclercq
// This file is part of the "Nazara Engine - Utility module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_ICONIMPL_HPP
#define NAZARA_ICONIMPL_HPP

#include <xcb/xcb.h>
#include <utility>

class NzImage;

class NzIconImpl
{
	public:
		bool Create(const NzImage& image);
		void Destroy();

		xcb_pixmap_t GetIcon();
		xcb_pixmap_t GetMask();

	private:
		xcb_pixmap_t m_icon_pixmap;
		xcb_pixmap_t m_mask_pixmap;
};

#endif // NAZARA_ICONIMPL_HPP