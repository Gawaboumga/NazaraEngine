// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#pragma once

#ifndef NAZARA_TMX_ENUMS_HPP
#define NAZARA_TMX_ENUMS_HPP

#include <Nazara/Core/Error.hpp>
#include <Nazara/Core/String.hpp>

namespace tmx
{
	enum AlignmentHorizontal
	{
		AlignmentHorizontal_Left,
		AlignmentHorizontal_Center,
		AlignmentHorizontal_Right,

		AlignmentHorizontal_Max = AlignmentHorizontal_Right
	};

	inline AlignmentHorizontal AlignmentHorizontalFromString(const Nz::String& string)
	{
		if (string == "left")
			return AlignmentHorizontal_Left;
		else if (string == "center")
			return AlignmentHorizontal_Center;
		else if (string == "right")
			return AlignmentHorizontal_Right;

		NazaraError("Unknown alignment horizontal: " + string);
		return AlignmentHorizontal_Max;
	}

	enum AlignmentVertical
	{
		AlignmentVertical_Bottom,
		AlignmentVertical_Center,
		AlignmentVertical_Top,

		AlignmentVertical_Max = AlignmentVertical_Top
	};

	inline AlignmentVertical AlignmentVerticalFromString(const Nz::String& string)
	{
		if (string == "bottom")
			return AlignmentVertical_Bottom;
		else if (string == "center")
			return AlignmentVertical_Center;
		else if (string == "top")
			return AlignmentVertical_Top;

		NazaraError("Unknown alignment horizontal: " + string);
		return AlignmentVertical_Max;
	}

	enum DrawOrder
	{
		DrawOrder_Index,
		DrawOrder_TopDown,

		DrawOrder_Max = DrawOrder_TopDown
	};

	inline DrawOrder DrawOrderFromString(const Nz::String& string)
	{
		if (string == "index")
			return DrawOrder_Index;
		else if (string == "topdown")
			return DrawOrder_TopDown;

		NazaraError("Unknown draw order: " + string);
		return DrawOrder_Max;
	}

	enum Orientation
	{
		Orientation_Hexagonal,
		Orientation_Isometric,
		Orientation_Orthogonal,
		Orientation_Staggered,

		Orientation_Max = Orientation_Staggered
	};

	inline Orientation OrientationFromString(const Nz::String& string)
	{
		if (string == "hexagonal")
			return Orientation_Hexagonal;
		else if (string == "isometric")
			return Orientation_Isometric;
		else if (string == "orthogonal")
			return Orientation_Orthogonal;
		else if (string == "staggered")
			return Orientation_Staggered;

		NazaraError("Unknown orientation: " + string);
		return Orientation_Max;
	}

	enum RenderOrder
	{
		RenderOrder_LeftDown,
		RenderOrder_LeftUp,
		RenderOrder_RightDown,
		RenderOrder_RightUp,

		RenderOrder_Max = RenderOrder_RightUp
	};

	inline RenderOrder RenderOrderFromString(const Nz::String& string)
	{
		if (string == "left-down")
			return RenderOrder_LeftDown;
		else if (string == "left-up")
			return RenderOrder_LeftUp;
		else if (string == "right-down")
			return RenderOrder_RightDown;
		else if (string == "right-up")
			return RenderOrder_RightUp;

		NazaraError("Unknown render order: " + string);
		return RenderOrder_Max;
	}
}

#endif // NAZARA_TMX_ENUMS_HPP
