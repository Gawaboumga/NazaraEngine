// Copyright (C) 2015 Jérôme Leclercq
// This file is part of the "Nazara Engine - Utility module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Utility/X11/CursorImpl.hpp>
#include <Nazara/Core/CallOnExit.hpp>
#include <Nazara/Core/Error.hpp>
#include <Nazara/Utility/Image.hpp>
#include <Nazara/Utility/PixelFormat.hpp>
#include <Nazara/Utility/X11/Display.hpp>
#include <X11/Xcursor/Xcursor.h>
#include <xcb/xcb_image.h>
#include <xcb/xcb_renderutil.h>
#include <Nazara/Utility/Debug.hpp>

bool NzCursorImpl::Create(const NzImage& cursor, int hotSpotX, int hotSpotY)
{
	NzImage cursorImage(cursor); // Vive le COW
	if (!cursorImage.Convert(nzPixelFormat_BGRA8))
	{
		NazaraError("Failed to convert cursor to BGRA8");
		return false;
	}

	auto width = cursorImage.GetWidth();
	auto height = cursorImage.GetHeight();

	Display* display = XOpenDisplay(nullptr);

	XcursorImage* xci = XcursorImageCreate(width, height);
	if (xci)
	{
		xci->xhot = hotSpotX;
		xci->yhot = hotSpotY;
		xci->delay = 0;
		auto it = reinterpret_cast<const nzUInt32*>(cursorImage.GetConstPixels());
		for (unsigned int i = 0; i < (width * height); i++)
		{
			xci->pixels[i] = *it;
			++it;
		}
		m_cursor = XcursorImageLoadCursor(display, xci);
		XcursorImageDestroy(xci);
		XCloseDisplay(display);
		return true;
	}
	else
	{
		NazaraError("Failed to create cursor image");
		XCloseDisplay(display);
		return false;
	}

	/* TODO add destroy on error
	xcb_connection_t* connection = X11::OpenConnection();

	xcb_screen_t* screen = X11::XCBDefaultScreen(connection);

	xcb_pixmap_t cursor_pixmap = 0;
	xcb_pixmap_t cursor_pixmap_mask = 0;
	xcb_gcontext_t cursorGC = 0;

	NzCallOnExit onExit([&]() {
		if (cursorGC)
		{
			if (!X11::CheckCookie(
				connection,
				xcb_free_gc(
					connection,
					cursorGC
				))
			)
				NazaraError("Failed to free cursor gc");

			cursorGC = 0;
		}

		if (cursor_pixmap)
		{
			if (!X11::CheckCookie(
				connection,
				xcb_free_pixmap(
					connection,
					cursor_pixmap
				))
			)
				NazaraError("Failed to free cursor pixmap");

			cursor_pixmap = 0;
		}

		if (cursor_pixmap_mask)
		{
			if (!X11::CheckCookie(
				connection,
				xcb_free_pixmap(
					connection,
					cursor_pixmap_mask
				))
			)
				NazaraError("Failed to free cursor pixmap mask");

			cursor_pixmap_mask = 0;
		}
	});

	cursor_pixmap = xcb_generate_id(connection);

	if (!X11::CheckCookie(
		connection,
		xcb_create_pixmap(
			connection,
			screen->root_depth,
            cursor_pixmap,
            screen->root,
            width,
            height
		)))
	{
		NazaraError("Failed to create cursor pixmap");
		return false;
	}

	cursorGC = xcb_generate_id(connection);

	if (!X11::CheckCookie(
		connection,
		xcb_create_gc(
			connection,
			cursorGC,
			cursor_pixmap,
            0,
            nullptr
		)))
	{
		NazaraError("Failed to create cursor gc");
		return false;
	}

	if (!X11::CheckCookie(
		connection,
        xcb_put_image(
            connection,
            XCB_IMAGE_FORMAT_Z_PIXMAP,
            cursor_pixmap,
            cursorGC,
            width,
            height,
            0,
            0,
            0,
            screen->root_depth,
            width * height * 4,
            cursorImage.GetConstPixels()
        )))
	{
		NazaraError("Failed to put image for cursor");
		return false;
	}

	if (!X11::CheckCookie(
		connection,
		xcb_free_gc(
			connection,
			cursorGC
		)))
	{
		NazaraError("Failed to free cursor gc");
		return false;
	}
	cursorGC = 0;

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
                    nzUInt8 opacity = (cursorImage.GetConstPixels()[(i * 8 + k + j * width) * 4 + 3] > 0) ? 1 : 0;
                    maskPixels[i + j * pitch] |= (opacity << k);
                }
            }
        }
    }

	cursor_pixmap_mask = xcb_create_pixmap_from_bitmap_data(
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

    if (!cursor_pixmap_mask)
	{
		NazaraError("Failed to create cursor mask");
		return false;
	}

    m_cursor = xcb_generate_id(connection);

    if (!X11::CheckCookie(
		connection,
		xcb_create_cursor(
			connection,
			m_cursor,
			cursor_pixmap,
			cursor_pixmap_mask,
			0, 0, 0,
			0, 0, 0,
			hotSpotX, hotSpotY
		)))
	{
		NazaraError("Failed to create cursor");
		return false;
	}

	if (!X11::CheckCookie(
		connection,
		xcb_free_pixmap(
			connection,
			cursor_pixmap
		)))
	{
		NazaraError("Failed to free cursor pixmap");
		return false;
	}
	cursor_pixmap = 0;

	if (!X11::CheckCookie(
		connection,
		xcb_free_pixmap(
			connection,
			cursor_pixmap_mask
		)))
	{
		NazaraError("Failed to free cursor pixmap mask");
		return false;
	}
	cursor_pixmap_mask = 0;

	onExit.Reset();

	X11::CloseConnection(connection);

	return true;*/
}

void NzCursorImpl::Destroy()
{
	xcb_connection_t* connection = X11::OpenConnection();

	xcb_free_cursor(connection, m_cursor);
	m_cursor = 0;

	X11::CloseConnection(connection);
}

xcb_cursor_t NzCursorImpl::GetCursor()
{
	return m_cursor;
}
