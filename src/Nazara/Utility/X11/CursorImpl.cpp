// Copyright (C) 2015 Jérôme Leclercq
// This file is part of the "Nazara Engine - Utility module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Utility/X11/CursorImpl.hpp>
#include <Nazara/Core/Error.hpp>
#include <Nazara/Utility/Image.hpp>
#include <Nazara/Utility/PixelFormat.hpp>
#include <Nazara/Utility/Debug.hpp>

bool NzCursorImpl::Create(const NzImage& cursor, int hotSpotX, int hotSpotY)
{
}

void NzCursorImpl::Destroy()
{
}

xcb_cursor_t NzCursorImpl::GetCursor()
{
}
