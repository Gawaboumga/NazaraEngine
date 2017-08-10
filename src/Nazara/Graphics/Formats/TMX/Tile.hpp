// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Graphics module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_LOADERS_TMX_TILE_HPP
#define NAZARA_LOADERS_TMX_TILE_HPP

#include <Nazara/Prerequesites.hpp>
#include <pugixml/pugixml.hpp>
#include <sstream>

namespace Nz
{
	namespace TMX
	{
		struct Tile
		{
			int id;
			int terrain[4];
			float probability;

			bool Parse(const pugi::xml_node& terrainNode)
			{
				id = terrainNode.attribute("id").as_int();
				probability = terrainNode.attribute("probability").as_float();

				const char* terrainType = terrainNode.attribute("terrain").value();
				std::stringstream ss(terrainType);

				int index = 0;
				int i;
				while (ss >> i)
				{
					terrain[index] = i;

					if (ss.peek() == ',')
						ss.ignore();
					++index;
				}

				return true;
			}
		};
	}
}

#endif // NAZARA_LOADERS_TMX_TILE_HPP
