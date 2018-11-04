// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#pragma once

#ifndef NAZARA_TMX_WANGSET_HPP
#define NAZARA_TMX_WANGSET_HPP

#include <Nazara/Core/Color.hpp>
#include <Nazara/Core/String.hpp>

namespace pugi
{
	class xml_node;
}

namespace tmx
{
	struct TileSetParams;
	class Visitor;

	class WangSet
	{
		public:
			WangSet(const pugi::xml_node& wangSetNode, const TileSetParams& params);

			void Accept(Visitor& visitor) const;

			struct WangCornerColor
			{
				Nz::String name;
				Nz::Color color;
				float probability;
				unsigned int tile;
			};

			struct WangEdgeColor
			{
				Nz::String name;
				Nz::Color color;
				float probability;
				unsigned int tile;
			};

			struct WangTile
			{
				unsigned int tileId;
				unsigned int wangId;
			};

			std::vector<WangCornerColor> wangCornerColors;
			std::vector<WangEdgeColor> wangEdgeColors;
			std::vector<WangTile> wangTiles;
			Nz::String name;
			unsigned int tile;

		private:
			void ParseWangCornerColor(const pugi::xml_node& wangCornerColorNode, const TileSetParams& params);
			void ParseWangEdgeColor(const pugi::xml_node& wangEdgeColorNode, const TileSetParams& params);
			void ParseWangTile(const pugi::xml_node& wangTileNode, const TileSetParams& params);
	};
}

#endif // NAZARA_TMX_WANGSET_HPP
