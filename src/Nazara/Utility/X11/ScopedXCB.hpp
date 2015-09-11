// Copyright (C) 2015 Jérôme Leclercq
// This file is part of the "Nazara Engine - Utility module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_SCOPEDXCB_HPP
#define NAZARA_SCOPEDXCB_HPP

#include <xcb/xcb_ewmh.h>

class xcb_connection_t;

class NzScopedXCBConnection
{
	public:
		NzScopedXCBConnection();
		~NzScopedXCBConnection();

		operator xcb_connection_t*() const;

	private:
		xcb_connection_t* m_connection;
};

class NzScopedXCBEWMHConnection
{
	public:
		NzScopedXCBEWMHConnection(xcb_connection_t* connection);
		~NzScopedXCBEWMHConnection();

		xcb_ewmh_connection_t* operator ->() const;

		operator xcb_ewmh_connection_t*() const;

	private:
		xcb_ewmh_connection_t* m_ewmhConnection;
};

template <typename T>
class NzScopedXCB
{
	public:
		NzScopedXCB(T* pointer);
		~NzScopedXCB();

		T* operator ->() const;
		T** operator &();

		operator bool() const;

		T* get() const;

	private:
		T* m_pointer;
};

#include <Nazara/Utility/X11/ScopedXCB.inl>

#endif // NAZARA_SCOPEDXCB_HPP
