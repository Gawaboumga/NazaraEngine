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

			struct Tile
			{
				Tile(unsigned int globalId);

				bool DiagonallyFlipped() const;
				unsigned int GetId() const;
				bool HorizontallyFlipped() const;
				bool VerticallyFlipped() const;

				unsigned int gid;

				const unsigned int FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
				const unsigned int FLIPPED_VERTICALLY_FLAG = 0x40000000;
				const unsigned int FLIPPED_DIAGONALLY_FLAG = 0x20000000;
			};

			std::vector<Property> properties;
			std::vector<Tile> tileIds;
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
