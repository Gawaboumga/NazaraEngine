// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Graphics module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_LOADERS_TMX_TILESET_HPP
#define NAZARA_LOADERS_TMX_TILESET_HPP

#include <Nazara/Prerequesites.hpp>
#include <Nazara/Graphics/Formats/TMX/Image.hpp>
#include <Nazara/Graphics/Formats/TMX/Terrain.hpp>
#include <Nazara/Graphics/Formats/TMX/Tile.hpp>
#include <pugixml/pugixml.hpp>

namespace Nz
{
	namespace TMX
	{
		struct TileSet
		{
			int firstGID;
			Nz::String name = "Default";
			Nz::Vector2f tileSize;
			int margin;
			int tileCount;
			int columns;

			Image image;
			std::vector<Terrain> terrains;
			std::vector<Tile> tiles;

			bool ParseTSX(const char* source, const TileMapParams& parameters)
			{
				std::unique_ptr<Stream> stream = parameters.loadTSX(source);
				if (!stream)
					return false;

				Nz::UInt64 streamSize = stream->GetSize();
				std::unique_ptr<char[]> buffer = std::make_unique<char[]>(streamSize);
				Nz::UInt64 read = stream->Read(buffer.get(), streamSize);
				if (read != streamSize)
				{
					NazaraError("Could not read the stream entirely");
					return false;
				}

				pugi::xml_document doc;
				pugi::xml_parse_result result = doc.load_buffer_inplace(buffer.get(), streamSize);

				if (!result)
				{
					NazaraError("Could not read the file: " + String(result.description()));
					return false;
				}

				return Parse(doc.child("tileset"), parameters);
			}

			bool Parse(const pugi::xml_node& tileSet, const TileMapParams& parameters)
			{
				firstGID = tileSet.attribute("firstgid").as_int();

				const char* source = tileSet.attribute("source").value();
				if (*source)
					return ParseTSX(source, parameters);

				name = tileSet.attribute("name").value();

				tileSize = Vector2f(tileSet.attribute("tilewidth").as_float(), tileSet.attribute("tileheight").as_float());
				margin = tileSet.attribute("margin").as_int();
				tileCount = tileSet.attribute("tilecount").as_int();
				columns = tileSet.attribute("columns").as_int();

				pugi::xml_node imageNode = tileSet.child("image");
				image.Parse(imageNode, parameters);

				pugi::xml_node terrainType = tileSet.child("terraintypes");
				for (pugi::xml_node terrainNode : terrainType.children("terrain"))
				{
					TMX::Terrain terrain;
					if (!terrain.Parse(terrainNode))
						return false;

					terrains.push_back(terrain);
				}

				for (pugi::xml_node tileNode : tileSet.children("tile"))
				{
					TMX::Tile tile;
					if (!tile.Parse(tileNode))
						return false;

					tiles.push_back(tile);
				}

				if (!tileSet.child("tile"))
				{
					int numberTiles = (image.imageSize.x / tileSize.x) * (image.imageSize.y / tileSize.y);
					for (int i = 0; i != numberTiles; ++i)
					{
						TMX::Tile tile;
						tile.id = i;
						tiles.push_back(tile);
					}
				}

				return true;
			}
		};
	}
}

#endif // NAZARA_LOADERS_TMX_TILESET_HPP
