// Copyright (C) 2015 Jérôme Leclercq
// This file is part of the "Nazara Engine - Utility module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Utility/X11/IconImpl.hpp>
#include <Nazara/Utility/Image.hpp>
#include <Nazara/Utility/PixelFormat.hpp>
#include <Nazara/Utility/X11/Display.hpp>
#include <xcb/xcb_image.h>
#include <Nazara/Utility/Debug.hpp>

NzIconImpl::NzIconImpl() :
m_icon_pixmap(0),
m_mask_pixmap(0)
{
}

bool NzIconImpl::Create(const NzImage& icon)
{
	NzImage iconImage(icon); // Vive le COW
	if (!iconImage.Convert(nzPixelFormat_BGRA8))
	{
		NazaraError("Failed to convert icon to BGRA8");
		return false;
	}

	auto width = iconImage.GetWidth();
	auto height = iconImage.GetHeight();

	NzScopedXCBConnection connection;

	xcb_screen_t* screen = X11::XCBDefaultScreen(connection);

	m_icon_pixmap = xcb_generate_id(connection);

	if (!X11::CheckCookie(
		connection,
		xcb_create_pixmap(
			connection,
			screen->root_depth,
            m_icon_pixmap,
            screen->root,
            width,
            height
		)))
	{
		NazaraError("Failed to create icon pixmap");
		return false;
	}

	xcb_gcontext_t iconGC = xcb_generate_id(connection);

	if (!X11::CheckCookie(
		connection,
		xcb_create_gc(
			connection,
			iconGC,
			m_icon_pixmap,
            0,
            nullptr
		)))
	{
		NazaraError("Failed to create icon gc");
		Destroy();
		return false;
	}

	if (!X11::CheckCookie(
		connection,
        xcb_put_image(
            connection,
            XCB_IMAGE_FORMAT_Z_PIXMAP,
            m_icon_pixmap,
            iconGC,
            width,
            height,
            0,
            0,
            0,
            screen->root_depth,
            width * height * 4,
            iconImage.GetConstPixels()
        )))
	{
		NazaraError("Failed to put image for icon");
		Destroy();
		return false;
	}

	if (!X11::CheckCookie(
		connection,
		xcb_free_gc(
			connection,
			iconGC
		)))
	{
		NazaraError("Failed to free icon gc");
		Destroy();
		return false;
	}

	// Create the mask pixmap (must have 1 bit depth)
    std::size_t pitch = (width + 7) / 8;
    static std::vector<nzUInt8> maskPixels(pitch * height, 0);
    for (std::size_t j = 0; j < height; ++j)
    {
        for (std::size_t i = 0; i < pitch; ++i)
        {
            for (std::size_t k = 0; k < 8; ++k)
            {
                if (i * 8 + k < width)
                {
                    nzUInt8 opacity = (iconImage.GetConstPixels()[(i * 8 + k + j * width) * 4 + 3] > 0) ? 1 : 0;
                    maskPixels[i + j * pitch] |= (opacity << k);
                }
            }
        }
    }

	m_mask_pixmap = xcb_create_pixmap_from_bitmap_data(
        connection,
        X11::XCBDefaultRootWindow(connection),
        reinterpret_cast<uint8_t*>(&maskPixels[0]),
        width,
        height,
        1,
        0,
        1,
        nullptr
    );

    if (!m_mask_pixmap)
	{
		NazaraError("Failed to create mask pixmap for icon");
		Destroy();
		return false;
	}

	return true;
}

void NzIconImpl::Destroy()
{
	NzScopedXCBConnection connection;

	if (m_icon_pixmap)
	{
		if (!X11::CheckCookie(
			connection,
			xcb_free_pixmap(
				connection,
				m_icon_pixmap
			))
		)
			NazaraError("Failed to free icon pixmap");

		m_icon_pixmap = 0;
	}

	if (m_mask_pixmap)
	{
		if (!X11::CheckCookie(
			connection,
			xcb_free_pixmap(
				connection,
				m_mask_pixmap
			))
		)
			NazaraError("Failed to free icon mask pixmap");

		m_mask_pixmap = 0;
	}
}

xcb_pixmap_t NzIconImpl::GetIcon()
{
	return m_icon_pixmap;
}

xcb_pixmap_t NzIconImpl::GetMask()
{
	return m_mask_pixmap;
}
