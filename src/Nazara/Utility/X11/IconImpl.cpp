// Copyright (C) 2015 Jérôme Leclercq
// This file is part of the "Nazara Engine - Utility module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Utility/X11/IconImpl.hpp>
#include <Nazara/Utility/Image.hpp>
#include <Nazara/Utility/PixelFormat.hpp>
#include <Nazara/Utility/Debug.hpp>

bool NzIconImpl::Create(const NzImage& icon)
{
	NzImage windowsIcon(icon); // Vive le COW
	if (!windowsIcon.Convert(nzPixelFormat_BGRA8))
	{
		NazaraError("Failed to convert icon to BGRA8");
		return false;
	}

	return true;
}

void NzIconImpl::Destroy()
{
}

xcb_pixmap_t NzIconImpl::GetIcon()
{
}
