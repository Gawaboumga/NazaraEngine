// Copyright (C) 2015 Jérôme Leclercq
// This file is part of the "Nazara Engine - Utility module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_X11DISPLAY_HPP
#define NAZARA_X11DISPLAY_HPP

#include <Nazara/Utility/WindowHandle.hpp>
#include <xcb/xcb_ewmh.h>
#include <string>

namespace X11
{
	xcb_connection_t* OpenConnection();
	xcb_ewmh_connection_t* OpenEWMHConnection(xcb_connection_t* connection);

	void CloseConnection(xcb_connection_t* connection);
	void CloseEWMHConnection(xcb_ewmh_connection_t* ewmh_connection);

	xcb_atom_t GetAtom(const std::string& name, bool onlyIfExists = false);

	xcb_screen_t* XCBDefaultScreen(xcb_connection_t* connection);
	xcb_window_t XCBDefaultRootWindow(xcb_connection_t* connection);
	xcb_screen_t* XCBScreenOfDisplay(xcb_connection_t* connection, int screen_nbr);

	bool TestCookie(xcb_connection_t* connection, xcb_void_cookie_t cookie, const std::string& message);
}

template <typename T>
class NzScopedXCB
{
	public:
		NzScopedXCB(T* pointer) :
		m_pointer(pointer) {}
		~NzScopedXCB() { std::free(m_pointer); }

		T* operator ->() const { return m_pointer; }
		T** operator &() { return &m_pointer; }

		operator bool() const { return m_pointer != nullptr; }

		T* get() const { return m_pointer; }

	private:
		T* m_pointer;
};

#endif // NAZARA_X11DISPLAY_HPP
