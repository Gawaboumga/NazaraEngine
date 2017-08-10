// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Graphics module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_LOADERS_TMX_MAP_HPP
#define NAZARA_LOADERS_TMX_MAP_HPP

#include <Nazara/Prerequesites.hpp>
#include <pugixml/pugixml.hpp>

namespace Nz
{
	namespace TMX
	{
		struct Map
		{
			enum Orientation
			{
				Orthogonal,
				Isometric,
				Staggered,
				Hexagonal
			};

			Orientation orientation;
			Nz::Vector2ui mapSize;
			Nz::Vector2f tileSize;

			bool Parse(const pugi::xml_node& map)
			{
				const char* orientationValue = map.attribute("orientation").value();
				orientation = Orthogonal;
				if (strcmp(orientationValue, "isometric"))
					orientation = Isometric;
				else if (strcmp(orientationValue, "staggered"))
					orientation = Staggered;
				else if (strcmp(orientationValue, "hexagonal"))
					orientation = Hexagonal;

				mapSize = Nz::Vector2ui(map.attribute("width").as_uint(), map.attribute("height").as_uint());
				tileSize = Nz::Vector2f(map.attribute("tilewidth").as_float(), map.attribute("tileheight").as_float());

				return true;
			}
		};
	}
}

#endif // NAZARA_LOADERS_TMX_MAP_HPP
