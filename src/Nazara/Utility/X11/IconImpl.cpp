// Copyright (C) 2015 Jérôme Leclercq
// This file is part of the "Nazara Engine - Utility module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Utility/X11/IconImpl.hpp>
#include <Nazara/Utility/Image.hpp>
#include <Nazara/Utility/PixelFormat.hpp>
#include <Nazara/Utility/X11/Display.hpp>
#include <xcb/xcb_image.h>
#include <Nazara/Utility/Debug.hpp>

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

	xcb_connection_t* connection = X11::OpenConnection();

	xcb_screen_t* screen = X11::XCBDefaultScreen(connection);

	m_icon_pixmap = xcb_generate_id(connection);

	X11::TestCookie(
		connection,
		xcb_create_pixmap(
			connection,
			screen->root_depth,
            m_icon_pixmap,
            screen->root,
            width,
            height
		), "Failed to create icon pixmap"
	);

	xcb_gcontext_t iconGC = xcb_generate_id(connection);

	X11::TestCookie(
		connection,
		xcb_create_gc(
			connection,
			iconGC,
			m_icon_pixmap,
            0,
            nullptr
		), "Failed to create icon gc"
	);

	X11::TestCookie(
		connection,
        xcb_put_image_checked(
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
        ), "Faild to put image for icon"
	);

	X11::TestCookie(
		connection,
		xcb_free_gc(
			connection,
			iconGC
		), "Failed to free icon gc"
	);

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
        NULL
    );

	X11::CloseConnection(connection);

	return true;
}

void NzIconImpl::Destroy()
{
	xcb_connection_t* connection = X11::OpenConnection();

	X11::TestCookie(
		connection,
        xcb_free_pixmap_checked(
            connection,
            m_icon_pixmap
        ), "Failed to free icon pixmap"
    );

	X11::TestCookie(
		connection,
        xcb_free_pixmap_checked(
            connection,
            m_mask_pixmap
        ), "Failed to free icon pixmap"
    );

    X11::CloseConnection(connection);
}

xcb_pixmap_t NzIconImpl::GetIcon()
{
	return m_icon_pixmap;
}

xcb_pixmap_t NzIconImpl::GetMask()
{
	return m_mask_pixmap;
}
