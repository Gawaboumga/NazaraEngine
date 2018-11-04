// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#pragma once

#ifndef NAZARA_TMX_TILESET_HPP
#define NAZARA_TMX_TILESET_HPP

#include <TMX/Enums.hpp>
#include <TMX/Property.hpp>
#include <TMX/Terrain.hpp>
#include <TMX/Tile.hpp>
#include <TMX/Wangset.hpp>

#include <Nazara/Core/Initializer.hpp>
#include <Nazara/Core/ObjectLibrary.hpp>
#include <Nazara/Core/ObjectRef.hpp>
#include <Nazara/Core/Resource.hpp>
#include <Nazara/Core/ResourceLoader.hpp>
#include <Nazara/Core/ResourceManager.hpp>
#include <Nazara/Core/ResourceParameters.hpp>
#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Renderer/Texture.hpp>

namespace pugi
{
	class xml_node;
}

namespace tmx
{
	struct TileSetParams : Nz::ResourceParameters
	{
		bool IsValid() const;

		PropertyParams propertyParams;
		Nz::String relativePath;
	};

	class TileSet;

	using TileSetConstRef = Nz::ObjectRef<const TileSet>;
	using TileSetLibrary = Nz::ObjectLibrary<TileSet>;
	using TileSetLoader = Nz::ResourceLoader<TileSet, TileSetParams>;
	using TileSetManager = Nz::ResourceManager<TileSet, TileSetParams>;
	using TileSetRef = Nz::ObjectRef<TileSet>;

	class TileSet : public Nz::RefCounted, public Nz::Resource
	{
		friend TileSetLibrary;
		friend TileSetLoader;
		friend TileSetManager;

		public:
			TileSet(const pugi::xml_node& tileSetNode, const TileSetParams& params = TileSetParams());

			void Accept(Visitor& visitor) const;

			static TileSetRef LoadFromFile(const Nz::String& filePath, const TileSetParams& params = TileSetParams());
			static TileSetRef LoadFromMemory(const void* data, std::size_t size, const TileSetParams& params = TileSetParams());
			static TileSetRef LoadFromStream(Nz::Stream& stream, const TileSetParams& params = TileSetParams());

			static bool Initialize();
			static void Uninitialize();

			template<typename... Args>
			static TileSetRef New(Args&&... args);

			struct Grid
			{
				Orientation orientation;
				Nz::Vector2ui size;
			};

			std::vector<Property> properties;
			std::vector<Terrain> terrains;
			std::vector<Tile> tiles;
			std::vector<WangSet> wangSets;
			std::unique_ptr<Grid> grid;
			Nz::TextureRef texture;
			Nz::String name;
			Nz::Vector2i offset;
			Nz::Vector2ui tileSize;
			unsigned int firstgid;
			unsigned int spacing;
			unsigned int margin;
			unsigned int tileCount;
			unsigned int columns;

		private:
			void Merge(const TileSetRef& otherTileSet);
			void ParseGrid(const pugi::xml_node& gridNode);
			void ParseImage(const pugi::xml_node& imageNode, const TileSetParams& params);

			static TileSetLibrary::LibraryMap s_library;
			static TileSetLoader::LoaderList s_loaders;
			static TileSetManager::ManagerMap s_managerMap;
			static TileSetManager::ManagerParams s_managerParameters;
	};
}

#include <TMX/TileSet.inl>

#endif // NAZARA_TMX_TILESET_HPP
