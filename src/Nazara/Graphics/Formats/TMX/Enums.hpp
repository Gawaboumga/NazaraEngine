// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Graphics module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_LOADERS_TMX_ENUMS_HPP
#define NAZARA_LOADERS_TMX_ENUMS_HPP

#include <Nazara/Prerequesites.hpp>

namespace Nz
{
	namespace TMX
	{
		enum Element
		{
			Element_Image,
			Element_Layer,
			Element_Map,
			Element_Property,
			Element_Terrain,
			Element_Tile,
			Element_TileSet,

			Element_Max = Element_TileSet
		};

	}
}

#endif // NAZARA_LOADERS_TMX_ENUMS_HPP
