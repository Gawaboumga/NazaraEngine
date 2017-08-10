// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Graphics module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Graphics/Formats/TMXLoader.hpp>
#include <Nazara/Core/CallOnExit.hpp>
#include <Nazara/Graphics/TileMap.hpp>
#include <Nazara/Graphics/Formats/TMX/Layer.hpp>
#include <Nazara/Graphics/Formats/TMX/Map.hpp>
#include <Nazara/Graphics/Formats/TMX/TileSet.hpp>
#include <pugixml/pugixml.hpp>
#include <algorithm>
#include <memory>
#include <Nazara/Graphics/Debug.hpp>

namespace Nz
{
	namespace
	{
		Vector2ui GetPosition(const TMX::Map& map, int i)
		{
			return Vector2ui(i % map.mapSize.x, i / map.mapSize.x);
		}

		Rectui GetSprite(const TMX::Map& map, const std::vector<TMX::TileSet>& tileSets, int id)
		{
			auto it = std::find_if(tileSets.rbegin(), tileSets.rend(), [&](const TMX::TileSet& tileSet) {
				return tileSet.firstGID <= id;
			});
			
			if (it == tileSets.rend())
				return Rectui::Zero();

			const TMX::TileSet& tileSet = *it;
			int value = id - 1;
			int numberOfTiles = tileSet.image.imageSize.x / tileSet.tileSize.x;
			int realX = value % numberOfTiles;
			int realY = value / numberOfTiles;
			int margin = tileSet.margin;
			return Rectui(
				realX * (tileSet.tileSize.x + 1 * margin) + margin,
				realY * (tileSet.tileSize.y + 1 * margin) + margin,
				tileSet.tileSize.x - 2 * margin,
				tileSet.tileSize.y - 2 * margin);
		}

		void Resolve(TileMap* tileMap, const TMX::Map& map, const std::vector<TMX::TileSet>& tileSets, const std::vector<TMX::Layer>& layers)
		{
			tileMap->Create(map.mapSize, map.tileSize, layers.size(), tileSets.size());

			for (int layer = 0; layer != layers.size(); ++layer)
			{
				for (int material = 0; material != tileSets.size(); ++material)
				{
					Nz::MaterialRef copy = tileSets[material].image.material;
					if (layer > 0)
					{
						copy->EnableBlending(true);
						copy->SetDstBlend(Nz::BlendFunc_InvSrcAlpha);
						copy->SetSrcBlend(Nz::BlendFunc_SrcAlpha);
					}
					tileMap->SetMaterial(layer, material, copy);
				}
			}

			for (auto const& tileSet : tileSets)
				NazaraError(String::Number(tileSet.firstGID));

			int layerIndex = 0;
			for (const TMX::Layer& layer : layers)
			{
				for (int i = 0; i != map.mapSize.x * map.mapSize.y; ++i)
				{
					if (layer.ids[i] == 0)
						continue;
					
					//if (layer.ids[i] < 0)
					//	NazaraError("???" + Nz::String::Number(layer.ids[i]));

					tileMap->EnableTile(
						layerIndex,
						GetPosition(map, i),
						GetSprite(map, tileSets, layer.ids[i])
					);
				}
				++layerIndex;
			}
		}

		bool IsSupported(const String& extension)
		{
			return (extension == "tmx");
		}

		Ternary Check(Stream& stream, const TileMapParams& parameters)
		{
			bool skip;
			if (parameters.custom.GetBooleanParameter("SkipNativeTileMapLoader", &skip) && skip)
				return Ternary_False;
			
			// Force stream in text mode, reset it at the end
			Nz::CallOnExit resetTextMode;
			if ((stream.GetStreamOptions() & StreamOption_Text) == 0)
			{
				stream.EnableTextMode(true);

				resetTextMode.Reset([&stream] ()
				{
					stream.EnableTextMode(false);
				});
			}

			Nz::String currentLine = stream.ReadLine();
			if (currentLine != R"(<?xml version="1.0" encoding="UTF-8"?>)")
				return Ternary_False;
			
			currentLine = stream.ReadLine(5);
			if (currentLine.StartsWith(R"(<map )"))
				return Ternary_True;

			return Ternary_Unknown;
		}

		bool Load(TileMap* tileMap, Stream& stream, const TileMapParams& parameters)
		{
			Nz::UInt64 streamSize = stream.GetSize();
			std::unique_ptr<char[]> buffer = std::make_unique<char[]>(streamSize);
			Nz::UInt64 read = stream.Read(buffer.get(), streamSize);
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

			pugi::xml_node map = doc.child("map");

			TMX::Map tmxMap;
			if (!tmxMap.Parse(map))
			{
				NazaraError("Could not read map");
				return false;
			}

			std::vector<TMX::TileSet> tileSets;
			for (pugi::xml_node tileSetNode : map.children("tileset"))
			{
				TMX::TileSet tileSet;
				if (!tileSet.Parse(tileSetNode, parameters))
				{
					NazaraError("Could not read tile set");
					return false;
				}
				tileSets.push_back(tileSet);
			}

			std::vector<TMX::Layer> layers;
			for (pugi::xml_node layerNode : map.children("layer"))
			{
				TMX::Layer layer;
				if (!layer.Parse(layerNode, tmxMap, parameters))
				{
					NazaraError("Could not read layer");
					return false;
				}
				layers.push_back(layer);
			}

			Resolve(tileMap, tmxMap, tileSets, layers);

			return true;
		}
	}

	namespace Loaders
	{
		void RegisterTMX()
		{
			TileMapLoader::RegisterLoader(IsSupported, Check, Load);
		}

		void UnregisterTMX()
		{
			TileMapLoader::UnregisterLoader(IsSupported, Check, Load);
		}
	}
}
