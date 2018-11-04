// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#include <TMX/Map.hpp>
#include <TMX/Enums.hpp>
#include <TMX/TMXLoader.hpp>
#include <TMX/Visitor.hpp>

#include <pugixml/src/pugixml.hpp>

namespace tmx
{
	bool MapParams::IsValid() const
	{
		return true;
	}

	void Map::Accept(Visitor& visitor) const
	{
		visitor.Enter(*this);

		for (const Group& group : groups)
			group.Accept(visitor);

		for (const ImageLayer& imageLayer : imageLayers)
			imageLayer.Accept(visitor);

		// It is important that tile set is seen before tile layer.
		for (const TileSetRef& tileSet : tileSets)
			tileSet->Accept(visitor);

		for (const TileLayer& tileLayer : tileLayers)
			tileLayer.Accept(visitor);

		for (const ObjectGroup& objectGroup : objectGroups)
			objectGroup.Accept(visitor);

		for (const Property& property : properties)
			property.Accept(visitor);

		visitor.Exit(*this);
	}

	MapRef Map::LoadFromFile(const Nz::String& filePath, const MapParams& params)
	{
		return MapLoader::LoadFromFile(filePath, params);
	}

	MapRef Map::LoadFromMemory(const void* data, std::size_t size, const MapParams& params)
	{
		return MapLoader::LoadFromMemory(data, size, params);
	}

	MapRef Map::LoadFromStream(Nz::Stream& stream, const MapParams& params)
	{
		return MapLoader::LoadFromStream(stream, params);
	}

	bool Map::Initialize()
	{
		if (!MapLibrary::Initialize())
		{
			NazaraError("Failed to initialise manager");
			return false;
		}

		if (!MapManager::Initialize())
		{
			NazaraError("Failed to initialise manager");
			return false;
		}

		Loaders::Register_tmxfile();

		return true;
	}

	void Map::Uninitialize()
	{
		MapManager::Uninitialize();
		MapLibrary::Uninitialize();

		Loaders::Unregister_tmxfile();
	}

	Map::Map(const pugi::xml_node& mapNode, const MapParams& params)
	{
		version = mapNode.attribute("version").as_string();
		tiledVersion = mapNode.attribute("tiledversion").as_string();

		Nz::String orientationString = mapNode.attribute("orientation").as_string();
		orientation = OrientationFromString(orientationString);

		Nz::String renderOrderString = mapNode.attribute("renderorder").as_string("right-down");
		renderOrder = RenderOrderFromString(renderOrderString);

		unsigned int width = mapNode.attribute("width").as_uint();
		unsigned int height = mapNode.attribute("height").as_uint();
		if (!width || !height)
			NazaraError("Invalid map tile sizes");
		size.Set(width, height);

		unsigned int tileWidth = mapNode.attribute("tilewidth").as_uint();
		unsigned int tileHeight = mapNode.attribute("tileheight").as_uint();
		if (!tileWidth || !tileHeight)
			NazaraError("Invalid tile sizes");
		tileSize.Set(tileWidth, tileHeight);

		hexSideLength = mapNode.attribute("hexsidelength").as_uint();
		staggerAxis = mapNode.attribute("staggeraxis").as_string()[0];
		Nz::String staggerIndex = mapNode.attribute("staggerindex").as_string();
		if (staggerIndex == "even")
			staggerIndexEven = true;
		else
			staggerIndexEven = false;

		Nz::String backgroundColorString = mapNode.attribute("backgroundcolor").as_string("#FFFFFF");
		backgroundColor = Nz::Color::FromHex(backgroundColorString);

		nextLayerId = mapNode.attribute("nextlayerid").as_uint();
		nextObjectId = mapNode.attribute("nextobjectid").as_uint();

		for (const auto& node : mapNode.children())
		{
			Nz::String name = node.name();
			if (name == "groups")
				groups.emplace_back(node, params);
			else if (name == "imagelayer")
				imageLayers.emplace_back(node, params);
			else if (name == "layer")
				tileLayers.emplace_back(node, params);
			else if (name == "objectgroup")
				objectGroups.emplace_back(node, params.propertyParams);
			else if (name == "properties")
				properties.emplace_back(node, params.propertyParams);
			else if (name == "tileset")
				tileSets.emplace_back(TileSet::New(node, params.tileSetParams));
			else
				NazaraError("Unidentified node: " + name);
		}
	}

	MapLibrary::LibraryMap Map::s_library;
	MapLoader::LoaderList Map::s_loaders;
	MapManager::ManagerMap Map::s_managerMap;
	MapManager::ManagerParams Map::s_managerParameters;
}
