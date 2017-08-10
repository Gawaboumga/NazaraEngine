// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Graphics module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_LOADERS_TMX_TERRAIN_HPP
#define NAZARA_LOADERS_TMX_TERRAIN_HPP

#include <Nazara/Prerequesites.hpp>
#include <pugixml/pugixml.hpp>

namespace Nz
{
	namespace TMX
	{
		struct Terrain
		{
			Nz::String name;
			int tile;

			bool Parse(const pugi::xml_node& terrain)
			{
				name = terrain.attribute("name").value();
				tile = terrain.attribute("tile").as_int();

				return true;
			}
		};
	}
}

#endif // NAZARA_LOADERS_TMX_TERRAIN_HPP
