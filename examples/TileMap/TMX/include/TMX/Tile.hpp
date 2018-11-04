// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#pragma once

#ifndef NAZARA_TMX_TILE_HPP
#define NAZARA_TMX_TILE_HPP

#include <TMX/ObjectGroup.hpp>
#include <TMX/Property.hpp>

namespace pugi
{
	class xml_node;
}

namespace tmx
{
	struct TileSetParams;

	class Tile
	{
		public:
			Tile(const pugi::xml_node& tileNode, const TileSetParams& params);

			void Accept(Visitor& visitor) const;

			std::vector<ObjectGroup> objectGroups;
			std::vector<Property> properties;
			Nz::String terrain;
			Nz::String type;
			float probability;
			unsigned int id;
	};
}

#endif // NAZARA_TMX_TILE_HPP
