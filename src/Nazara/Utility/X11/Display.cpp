// Copyright (C) 2015 Jérôme Leclercq
// This file is part of the "Nazara Engine - Utility module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Utility/X11/Display.hpp>
#include <Nazara/Core/Error.hpp>
#include <cassert>
#include <map>
#include <xcb/xcb_keysyms.h>
#include <Nazara/Utility/Debug.hpp>

namespace
{
	// The shared display and its reference counter
	xcb_connection_t* sharedConnection = nullptr;
	int screen_nbr = 0;
	unsigned int referenceCountConnection = 0;

	xcb_key_symbols_t* sharedkeySymbol;
	unsigned int referenceCountKeySymbol = 0;

	xcb_ewmh_connection_t* sharedEwmhConnection;
	unsigned int referenceCountEwmhConnection = 0;

	using AtomMap = std::map<std::string, xcb_atom_t>;
	AtomMap atoms;
}

namespace X11
{
	bool CheckCookie(xcb_connection_t* connection, xcb_void_cookie_t cookie)
	{
		NzScopedXCB<xcb_generic_error_t> error(xcb_request_check(
			connection,
			cookie
		));

		if (error)
			return false;
		else
			return true;
	}

	void CloseConnection(xcb_connection_t* connection)
	{
		assert(connection == sharedConnection);
		--referenceCountConnection;
	}

	void CloseEWMHConnection(xcb_ewmh_connection_t* ewmh_connection)
	{
		assert(ewmh_connection == sharedEwmhConnection);
		--referenceCountEwmhConnection;
	}

	xcb_atom_t GetAtom(const std::string& name, bool onlyIfExists)
	{
		AtomMap::const_iterator iter = atoms.find(name);

		if (iter != atoms.end())
			return iter->second;

		NzScopedXCB<xcb_generic_error_t> error(nullptr);

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

	void Initialize()
	{
		assert(referenceCountConnection == 0);
		assert(referenceCountKeySymbol == 0);
		assert(referenceCountEwmhConnection == 0);

		{
			sharedConnection = xcb_connect(nullptr, &screen_nbr);

			// Opening display failed: The best we can do at the moment is to output a meaningful error message
			// and cause an abnormal program termination
			if (!sharedConnection || xcb_connection_has_error(sharedConnection))
			{
				NazaraError("Failed to open xcb connection");
				std::abort();
			}

			OpenConnection();
		}

		{
			sharedkeySymbol = xcb_key_symbols_alloc(sharedConnection);

			XCBKeySymbolsAlloc(sharedConnection);
		}

		{
			sharedEwmhConnection = new xcb_ewmh_connection_t;
			xcb_intern_atom_cookie_t* ewmh_cookie = xcb_ewmh_init_atoms(sharedConnection, sharedEwmhConnection);

			if(!xcb_ewmh_init_atoms_replies(sharedEwmhConnection, ewmh_cookie, nullptr))
			{
				NazaraError("Could not initialize EWMH Connection");
				sharedEwmhConnection = nullptr;
			}

			OpenEWMHConnection(sharedConnection);
		}
	}

	xcb_key_symbols_t* XCBKeySymbolsAlloc(xcb_connection_t* connection)
	{
		assert(connection == sharedConnection);

		++referenceCountKeySymbol;
		return sharedkeySymbol;
	}

	void XCBKeySymbolsFree(xcb_key_symbols_t* keySymbols)
	{
		assert(keySymbols == sharedkeySymbol);

		--referenceCountKeySymbol;
	}

	xcb_connection_t* OpenConnection()
	{
		++referenceCountConnection;
		return sharedConnection;
	}

	xcb_ewmh_connection_t* OpenEWMHConnection(xcb_connection_t* connection)
	{
		assert(connection == sharedConnection);

		++referenceCountEwmhConnection;
		return sharedEwmhConnection;
	}

	void Uninitialize()
	{
		{
			assert(referenceCountEwmhConnection == 1);
			CloseEWMHConnection(sharedEwmhConnection);

			xcb_ewmh_connection_wipe(sharedEwmhConnection);
			delete sharedEwmhConnection;
		}

		{
			assert(referenceCountKeySymbol == 1);
			XCBKeySymbolsFree(sharedkeySymbol);

			xcb_key_symbols_free(sharedkeySymbol);
		}

		{
			assert(referenceCountConnection == 1);
			CloseConnection(sharedConnection);

			xcb_disconnect(sharedConnection);
		}
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

	int XCBScreen(xcb_connection_t* connection)
	{
		assert(connection == sharedConnection);
		return screen_nbr;
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
