// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#pragma once

#ifndef NAZARA_TMX_TERRAIN_HPP
#define NAZARA_TMX_TERRAIN_HPP

#include <TMX/Property.hpp>

namespace pugi
{
	class xml_node;
}

namespace tmx
{
	struct TileSetParams;

	class Terrain
	{
		public:
			Terrain(const pugi::xml_node& terrainNode, const TileSetParams& params);

			void Accept(Visitor& visitor) const;

			std::vector<Property> properties;
			Nz::String name;
			unsigned int tile;
	};
}

#endif // NAZARA_TMX_TERRAIN_HPP
