// Copyright (C) 2015 Jérôme Leclercq
// This file is part of the "Nazara Engine - Utility module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Utility/X11/ScopedXCB.hpp>
#include <Nazara/Utility/X11/Display.hpp>
#include <Nazara/Utility/Debug.hpp>

NzScopedXCBConnection::NzScopedXCBConnection() :
m_connection(nullptr)
{
	m_connection = X11::OpenConnection();
}

NzScopedXCBConnection::~NzScopedXCBConnection()
{
	X11::CloseConnection(m_connection);
}

NzScopedXCBConnection::operator xcb_connection_t*() const
{
	return m_connection;
}

NzScopedXCBEWMHConnection::NzScopedXCBEWMHConnection(xcb_connection_t* connection) :
m_ewmhConnection(nullptr)
{
	m_ewmhConnection = X11::OpenEWMHConnection(connection);
}

NzScopedXCBEWMHConnection::~NzScopedXCBEWMHConnection()
{
	X11::CloseEWMHConnection(m_ewmhConnection);
}

xcb_ewmh_connection_t* NzScopedXCBEWMHConnection::operator ->() const
{
	return m_ewmhConnection;
}

NzScopedXCBEWMHConnection::operator xcb_ewmh_connection_t*() const
{
	return m_ewmhConnection;
}
