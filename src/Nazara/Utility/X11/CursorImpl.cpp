// Copyright (C) 2015 Jérôme Leclercq
// This file is part of the "Nazara Engine - Utility module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Utility/X11/CursorImpl.hpp>
#include <Nazara/Core/Error.hpp>
#include <Nazara/Utility/Image.hpp>
#include <Nazara/Utility/PixelFormat.hpp>
#include <Nazara/Utility/X11/Display.hpp>
#include <xcb/xcb_image.h>
#include <iostream>
#include <Nazara/Utility/Debug.hpp>

bool NzCursorImpl::Create(const NzImage& cursor, int hotSpotX, int hotSpotY)
{
	NzImage windowsCursor(cursor);
	if (!windowsCursor.Convert(nzPixelFormat_BGRA8))
	{
		NazaraError("Failed to convert cursor to BGRA8");
		return false;
	}

	xcb_connection_t* connection = X11::OpenConnection();
	xcb_screen_t* screen = X11::XCBDefaultScreen(connection);

	xcb_pixmap_t cursorPixmap = xcb_create_pixmap_from_bitmap_data(
		connection,
		screen->root,
		const_cast<nzUInt8*>(windowsCursor.GetConstPixels()),
		windowsCursor.GetWidth(),
		windowsCursor.GetHeight(),
		windowsCursor.GetDepth(),
		0, 0,
		0);

	if (!cursorPixmap)
	{
		NazaraError("Failed to create cursor pixmap");
		return false;
	}

	m_cursor = xcb_generate_id(connection);

	if (!X11::TestCookie(
		connection,
		xcb_create_cursor(
			connection,
			m_cursor,
			cursorPixmap,
			cursorPixmap,
			0, 0, 0, // Foreground RGB color
			0xFFFF, 0xFFFF, 0xFFFF, // Background RGB color
			hotSpotX, // X
			hotSpotY  // Y
		), "Failed to create hidden cursor"
	))
		return false;

	// We don't need the pixmap any longer, free it
	if (!X11::TestCookie(
		connection,
		xcb_free_pixmap(
			connection,
			cursorPixmap
		), "Failed to free pixmap for cursor"
	))
		return false;

	X11::CloseConnection(connection);

	return true;
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
