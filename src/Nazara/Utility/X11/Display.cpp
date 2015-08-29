// Copyright (C) 2015 Jérôme Leclercq
// This file is part of the "Nazara Engine - Utility module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Utility/X11/Display.hpp>
#include <Nazara/Core/Error.hpp>
#include <cassert>
#include <map>
#include <Nazara/Utility/Debug.hpp>

namespace
{
	// The shared display and its reference counter
	xcb_connection_t* sharedConnection = nullptr;
	int screen_nbr = 0;
	unsigned int referenceCount = 0;

	using AtomMap = std::map<std::string, xcb_atom_t>;
	AtomMap atoms;
}

namespace X11
{
	xcb_connection_t* OpenConnection()
	{
		if (referenceCount == 0)
		{
			sharedConnection = xcb_connect(nullptr, &screen_nbr);

			// Opening display failed: The best we can do at the moment is to output a meaningful error message
			// and cause an abnormal program termination
			if (!sharedConnection)
			{
				NazaraError("Failed to open xcb connection");
				std::abort();
			}
		}

		referenceCount++;
		return sharedConnection;
	}

	xcb_ewmh_connection_t* OpenEWMHConnection(xcb_connection_t* connection)
	{
		assert(connection = sharedConnection);
		xcb_ewmh_connection_t* ewmh_connection = new xcb_ewmh_connection_t;
		xcb_intern_atom_cookie_t* ewmh_cookie = xcb_ewmh_init_atoms(connection, ewmh_connection);

		if(!xcb_ewmh_init_atoms_replies(ewmh_connection, ewmh_cookie, NULL))
		{
			NazaraError("Could not initialize EWMH Connection");
			return nullptr;
		}

		return ewmh_connection;
	}

	void CloseConnection(xcb_connection_t* connection)
	{
		assert(connection == sharedConnection);
		if (--referenceCount == 0)
			xcb_disconnect(sharedConnection);
	}

	void CloseEWMHConnection(xcb_ewmh_connection_t* ewmh_connection)
	{
		xcb_ewmh_connection_wipe(ewmh_connection);
		delete ewmh_connection;
	}

	xcb_atom_t GetAtom(const std::string& name, bool onlyIfExists)
	{
		AtomMap::const_iterator iter = atoms.find(name);

		if (iter != atoms.end())
			return iter->second;

		NzScopedXCB<xcb_generic_error_t> error(NULL);

		xcb_connection_t* connection = OpenConnection();

		NzScopedXCB<xcb_intern_atom_reply_t> reply(xcb_intern_atom_reply(
			connection,
			xcb_intern_atom(
				connection,
				onlyIfExists,
				name.size(),
				name.c_str()
			),
			&error
		));

		CloseConnection(connection);

		if (error || !reply)
		{
			NazaraError("Failed to get " + name + " atom.");
			return XCB_ATOM_NONE;
		}

		atoms[name] = reply->atom;

		return reply->atom;
	}

	xcb_window_t XCBDefaultRootWindow(xcb_connection_t* connection)
	{
		assert(connection == sharedConnection);
		xcb_screen_t* screen = XCBDefaultScreen(connection);
		if (screen)
			return screen->root;
		return 0;
	}

	xcb_screen_t* XCBDefaultScreen(xcb_connection_t* connection)
	{
		assert(connection == sharedConnection);
		return XCBScreenOfDisplay(connection, screen_nbr);
	}

	xcb_screen_t* XCBScreenOfDisplay(xcb_connection_t* connection, int screen_nbr)
	{
		assert(connection == sharedConnection);
		xcb_screen_iterator_t iter = xcb_setup_roots_iterator(xcb_get_setup(connection));

		for (; iter.rem; --screen_nbr, xcb_screen_next (&iter))
		{
			if (screen_nbr == 0)
				return iter.data;
		}

		return nullptr;
	}
}
