// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#pragma once

#ifndef NAZARA_TMX_TILE_LAYER_HPP
#define NAZARA_TMX_TILE_LAYER_HPP

#include <TMX/Property.hpp>

#include <Nazara/Math/Vector2.hpp>

namespace pugi
{
	class xml_node;
}

namespace tmx
{
	struct MapParams;

	class TileLayer
	{
		public:
			TileLayer(const pugi::xml_node& tileLayerNode, const MapParams& params);

			void Accept(Visitor& visitor) const;

			std::vector<Property> properties;
			std::vector<unsigned int> tileIds;
			Nz::String name;
			Nz::Vector2i offset;
			Nz::Vector2i position;
			Nz::Vector2ui size;
			float opacity;
			unsigned int id;
			bool visible;

		private:
			void ParseBase64(const pugi::xml_node& node);
			void ParseCSV(const pugi::xml_node& node);
			void ParseUnencoded(const pugi::xml_node& node);
	};
}

#endif // NAZARA_TMX_TILE_LAYER_HPP
