// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#pragma once

#ifndef NAZARA_TMX_MAP_HPP
#define NAZARA_TMX_MAP_HPP

#include <Nazara/Core/Initializer.hpp>
#include <Nazara/Core/ObjectLibrary.hpp>
#include <Nazara/Core/ObjectRef.hpp>
#include <Nazara/Core/Resource.hpp>
#include <Nazara/Core/ResourceLoader.hpp>
#include <Nazara/Core/ResourceManager.hpp>
#include <Nazara/Core/ResourceParameters.hpp>
#include <Nazara/Math/Vector2.hpp>

#include <TMX/Enums.hpp>
#include <TMX/Group.hpp>
#include <TMX/ImageLayer.hpp>
#include <TMX/ObjectGroup.hpp>
#include <TMX/Property.hpp>
#include <TMX/TileLayer.hpp>
#include <TMX/Tileset.hpp>

namespace pugi
{
	class xml_node;
}

namespace tmx
{
	struct MapParams : Nz::ResourceParameters
	{
		bool IsValid() const;

		TileSetParams tileSetParams;
		PropertyParams propertyParams;
	};

	class Map;
	class Visitor;

	using MapConstRef = Nz::ObjectRef<const Map>;
	using MapLibrary = Nz::ObjectLibrary<Map>;
	using MapLoader = Nz::ResourceLoader<Map, MapParams>;
	using MapManager = Nz::ResourceManager<Map, MapParams>;
	using MapRef = Nz::ObjectRef<Map>;

	class Map : public Nz::RefCounted, public Nz::Resource
	{
		friend MapLibrary;
		friend MapLoader;
		friend MapManager;

		public:
			void Accept(Visitor& visitor) const;

			static MapRef LoadFromFile(const Nz::String& filePath, const MapParams& params = MapParams());
			static MapRef LoadFromMemory(const void* data, std::size_t size, const MapParams& params = MapParams());
			static MapRef LoadFromStream(Nz::Stream& stream, const MapParams& params = MapParams());

			static bool Initialize();
			static void Uninitialize();

			template<typename... Args>
			static MapRef New(Args&&... args);

			std::vector<Group> groups;
			std::vector<ImageLayer> imageLayers;
			std::vector<TileLayer> tileLayers;
			std::vector<TileSetRef> tileSets;
			std::vector<ObjectGroup> objectGroups;
			std::vector<Property> properties;
			Nz::Color backgroundColor;
			Nz::String version;
			Nz::String tiledVersion;
			Nz::Vector2ui size;
			Nz::Vector2ui tileSize;
			Orientation orientation;
			RenderOrder renderOrder;
			unsigned int hexSideLength;
			unsigned int nextLayerId;
			unsigned int nextObjectId;
			char staggerAxis;
			bool staggerIndexEven;

		private:
			Map(const pugi::xml_node& mapNode, const MapParams& params = MapParams());

			static MapLibrary::LibraryMap s_library;
			static MapLoader::LoaderList s_loaders;
			static MapManager::ManagerMap s_managerMap;
			static MapManager::ManagerParams s_managerParameters;
	};
}

#include <TMX/Map.inl>

#endif // NAZARA_TMX_MAP_HPP
