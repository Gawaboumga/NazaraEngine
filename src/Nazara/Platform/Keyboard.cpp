// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Platform module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Platform/Keyboard.hpp>
#include <Nazara/Platform/SDL2/InputImpl.hpp>

#include <Nazara/Platform/Debug.hpp>

namespace Nz
{
	String Keyboard::GetKeyName(Key key)
	{
		return EventImpl::GetKeyName(key);
	}

	bool Keyboard::IsKeyPressed(Key key)
	{
		return EventImpl::IsKeyPressed(key);
	}
}
