// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#include <TMX/TMXLoader.hpp>
#include <TMX/Map.hpp>
#include <TMX/TileSet.hpp>

#include <pugixml/src/pugixml.hpp>

namespace tmx
{
	namespace Detail
	{
		bool IsSupportedMap(const Nz::String& extension)
		{
			return extension == "tmx";
		}

		bool IsSupportedTileSet(const Nz::String& extension)
		{
			return extension == "tsx";
		}

		Nz::Ternary CheckMap(Nz::Stream& stream, const MapParams& parameters)
		{
			NazaraUnused(stream);
			NazaraUnused(parameters);
			return Nz::Ternary_True;
		}

		Nz::Ternary CheckTileSet(Nz::Stream& stream, const TileSetParams& parameters)
		{
			NazaraUnused(stream);
			NazaraUnused(parameters);
			return Nz::Ternary_True;
		}

		MapRef LoadStreamMap(Nz::Stream& stream, const MapParams& parameters)
		{
			/*pugi::xml_document doc;
			doc.
			return Map::Load(result);*/
			return MapRef();
		}

		MapRef LoadFileMap(const Nz::String& filePath, const MapParams& parameters)
		{
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(filePath.GetConstBuffer());
			pugi::xml_node mapNode = doc.child("map");
			return Map::New(mapNode, parameters);
		}

		MapRef LoadMemoryMap(const void* data, std::size_t size, const MapParams& parameters)
		{
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_buffer(data, size);
			pugi::xml_node mapNode = doc.child("map");
			return Map::New(mapNode, parameters);
		}

		TileSetRef LoadStreamTileSet(Nz::Stream& stream, const TileSetParams& parameters)
		{
			return TileSetRef();
		}

		TileSetRef LoadFileTileSet(const Nz::String& filePath, const TileSetParams& parameters)
		{
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(filePath.GetConstBuffer());
			pugi::xml_node tileSetNode = doc.child("tileset");
			return TileSet::New(tileSetNode, parameters);
		}

		TileSetRef LoadMemoryTileSet(const void* data, std::size_t size, const TileSetParams& parameters)
		{
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_buffer(data, size);
			pugi::xml_node tileSetNode = doc.child("tileset");
			return TileSet::New(tileSetNode, parameters);
		}
	}

	namespace Loaders
	{
		void Register_tmxfile()
		{
			MapLoader::RegisterLoader(Detail::IsSupportedMap, Detail::CheckMap, Detail::LoadStreamMap, Detail::LoadFileMap, Detail::LoadMemoryMap);
			TileSetLoader::RegisterLoader(Detail::IsSupportedTileSet, Detail::CheckTileSet, Detail::LoadStreamTileSet, Detail::LoadFileTileSet, Detail::LoadMemoryTileSet);
		}

		void Unregister_tmxfile()
		{
			MapLoader::UnregisterLoader(Detail::IsSupportedMap, Detail::CheckMap, Detail::LoadStreamMap, Detail::LoadFileMap, Detail::LoadMemoryMap);
			TileSetLoader::UnregisterLoader(Detail::IsSupportedTileSet, Detail::CheckTileSet, Detail::LoadStreamTileSet, Detail::LoadFileTileSet, Detail::LoadMemoryTileSet);
		}
	}
}
