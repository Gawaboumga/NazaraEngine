// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#include <TMX/TileSet.hpp>
#include <TMX/Enums.hpp>
#include <TMX/Visitor.hpp>

#include <pugixml/src/pugixml.hpp>

namespace tmx
{
	bool TileSetParams::IsValid() const
	{
		return true;
	}

	TileSet::TileSet(const pugi::xml_node& tileSetNode, const TileSetParams& params)
	{
		firstgid = tileSetNode.attribute("firstgid").as_uint();
		TileSetRef otherTileSet;

		Nz::String source = tileSetNode.attribute("source").as_string();
		if (!source.IsEmpty())
			otherTileSet = LoadFromFile(params.relativePath + source, params);
		name = tileSetNode.attribute("name").as_string();

		unsigned int tileWidth = tileSetNode.attribute("tilewidth").as_uint();
		unsigned int tileHeight = tileSetNode.attribute("tileheight").as_uint();
		tileSize.Set(tileWidth, tileHeight);

		spacing = tileSetNode.attribute("spacing").as_uint();
		margin = tileSetNode.attribute("margin").as_uint();
		tileCount = tileSetNode.attribute("tilecount").as_uint();
		columns = tileSetNode.attribute("columns").as_uint();

		pugi::xml_node tileOffsetNode = tileSetNode.child("tileoffset");
		if (tileOffsetNode)
		{
			int offsetX = tileOffsetNode.attribute("x").as_int();
			int offsetY = tileOffsetNode.attribute("y").as_int();
			offset.Set(offsetX, offsetY);
		}
		else
			offset = Nz::Vector2i::Zero();

		for (const auto& node : tileSetNode.children())
		{
			Nz::String name = node.name();
			if (name == "grid")
				ParseGrid(node);
			else if (name == "image")
				ParseImage(node, params);
			else if (name == "properties")
				properties.emplace_back(node, params.propertyParams);
			else if (name == "terraintypes")
			{
				for (const auto& terrainNode : node.children())
				{
					Nz::String name = terrainNode.name();
					if (name == "terrain")
						terrains.emplace_back(terrainNode, params);
					else
						NazaraError("Unidentified node: " + name);
				}
			}
				
			else if (name == "tile")
				tiles.emplace_back(node, params);
			else if (name == "wangsets")
			{
				for (const auto& wangSetNode : node.children())
				{
					Nz::String name = wangSetNode.name();
					if (name == "wangset")
						wangSets.emplace_back(wangSetNode, params);
					else
						NazaraError("Unidentified node: " + name);
				}
			}
			else
				NazaraError("Unidentified node: " + name);
		}

		if (otherTileSet)
			Merge(otherTileSet);
	}

	void TileSet::Accept(Visitor& visitor) const
	{
		visitor.Enter(*this);

		for (const Property& property : properties)
			property.Accept(visitor);

		for (const Terrain& terrain : terrains)
			terrain.Accept(visitor);

		for (const Tile& tile : tiles)
			tile.Accept(visitor);

		for (const WangSet& wangSet : wangSets)
			wangSet.Accept(visitor);

		visitor.Exit(*this);
	}

	TileSetRef TileSet::LoadFromFile(const Nz::String& filePath, const TileSetParams& params)
	{
		return TileSetLoader::LoadFromFile(filePath, params);
	}

	TileSetRef TileSet::LoadFromMemory(const void* data, std::size_t size, const TileSetParams& params)
	{
		return TileSetLoader::LoadFromMemory(data, size, params);
	}

	TileSetRef TileSet::LoadFromStream(Nz::Stream& stream, const TileSetParams& params)
	{
		return TileSetLoader::LoadFromStream(stream, params);
	}

	bool TileSet::Initialize()
	{
		if (!TileSetLibrary::Initialize())
		{
			NazaraError("Failed to initialise manager");
			return false;
		}

		if (!TileSetManager::Initialize())
		{
			NazaraError("Failed to initialise manager");
			return false;
		}

		return true;
	}

	void TileSet::Uninitialize()
	{
		TileSetManager::Uninitialize();
		TileSetLibrary::Uninitialize();
	}

	void TileSet::Merge(const TileSetRef& otherTileSet)
	{
		name = otherTileSet->name;
		tileSize = otherTileSet->tileSize;
		spacing = otherTileSet->spacing;
		margin = otherTileSet->margin;
		tileCount = otherTileSet->tileCount;
		columns = otherTileSet->columns;
		offset = otherTileSet->offset;

		properties = std::move(otherTileSet->properties);
		terrains = std::move(otherTileSet->terrains);
		tiles = std::move(otherTileSet->tiles);
		wangSets = std::move(otherTileSet->wangSets);
		grid = std::move(otherTileSet->grid);
		texture = std::move(otherTileSet->texture);
	}

	void TileSet::ParseGrid(const pugi::xml_node& gridNode)
	{
		Nz::String orientationString = gridNode.attribute("orientation").as_string();
		Orientation orientation = OrientationFromString(orientationString);
		unsigned int width = gridNode.attribute("width").as_uint();
		unsigned int height = gridNode.attribute("height").as_uint();

		grid = std::make_unique<Grid>();
		grid->orientation = orientation;
		grid->size.Set(width, height);
	}

	void TileSet::ParseImage(const pugi::xml_node& imageNode, const TileSetParams& params)
	{
		texture = Nz::Texture::LoadFromFile(params.relativePath + imageNode.attribute("source").as_string());
	}

	TileSetLibrary::LibraryMap TileSet::s_library;
	TileSetLoader::LoaderList TileSet::s_loaders;
	TileSetManager::ManagerMap TileSet::s_managerMap;
	TileSetManager::ManagerParams TileSet::s_managerParameters;
}
