// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Graphics module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_TILEMAP_HPP
#define NAZARA_TILEMAP_HPP

#include <Nazara/Prerequesites.hpp>
#include <Nazara/Core/Resource.hpp>
#include <Nazara/Core/ResourceLoader.hpp>
#include <Nazara/Core/ResourceParameters.hpp>
#include <Nazara/Graphics/InstancedRenderable.hpp>
#include <Nazara/Graphics/Material.hpp>
#include <Nazara/Utility/VertexStruct.hpp>
#include <array>
#include <set>

namespace Nz
{
	struct NAZARA_GRAPHICS_API TileMapParams : ResourceParameters
	{
		TileMapParams();

		std::function<MaterialRef(const Nz::String& /* name */)> loadImage;
		std::function<std::unique_ptr<Stream>(const Nz::String& /* name */)> loadTSX;
		Nz::String pathRoot;

		bool IsValid() const;
	};

	class TileMap;

	using TileMapConstRef = ObjectRef<const TileMap>;
	using TileMapLibrary = ObjectLibrary<TileMap>;
	using TileMapLoader = ResourceLoader<TileMap, TileMapParams>;
	using TileMapRef = ObjectRef<TileMap>;

	class NAZARA_GRAPHICS_API TileMap : public InstancedRenderable, public Resource
	{
		friend TileMapLibrary;
		friend TileMapLoader;
		friend class Graphics;

		public:
			struct Tile;

			TileMap() = default;
			inline TileMap(const Nz::Vector2ui& mapSize, const Nz::Vector2f& tileSize, std::size_t layerCount = 1, std::size_t materialCount = 1);
			TileMap(const TileMap& TileMap) = default;
			TileMap(TileMap&&) = delete;
			~TileMap() = default;

			void AddToRenderQueue(AbstractRenderQueue* renderQueue, const InstanceData& instanceData) const override;

			void Create(const Nz::Vector2ui& mapSize, const Nz::Vector2f& tileSize, std::size_t layerCount = 1, std::size_t materialCount = 1);

			inline void DisableTile(std::size_t layer, const Vector2ui& tilePos);
			inline void DisableTiles();
			inline void DisableTiles(std::size_t layer);
			inline void DisableTiles(std::size_t layer, const Vector2ui* tilesPos, std::size_t tileCount);

			inline void EnableIsometricMode(bool isometric);

			inline void EnableTile(std::size_t layer, const Vector2ui& tilePos, const Rectf& coords, const Color& color = Color::White, std::size_t materialIndex = 0U);
			inline void EnableTile(std::size_t layer, const Vector2ui& tilePos, const Rectui& rect, const Color& color = Color::White, std::size_t materialIndex = 0U);
			inline void EnableTiles(std::size_t layer, const Rectf& coords, const Color& color = Color::White, std::size_t materialIndex = 0U);
			inline void EnableTiles(std::size_t layer, const Rectui& rect, const Color& color = Color::White, std::size_t materialIndex = 0U);
			inline void EnableTiles(std::size_t layer, const Vector2ui* tilesPos, std::size_t tileCount, const Rectf& coords, const Color& color = Color::White, std::size_t materialIndex = 0U);
			inline void EnableTiles(std::size_t layer, const Vector2ui* tilesPos, std::size_t tileCount, const Rectui& rect, const Color& color = Color::White, std::size_t materialIndex = 0U);

			inline const MaterialRef& GetMaterial(std::size_t layer, std::size_t index) const;
			inline std::size_t GetMaterialCount() const;
			inline const Vector2ui& GetMapSize() const;
			inline Vector2f GetSize() const;
			inline const Tile& GetTile(std::size_t layer, const Vector2ui& tilePos) const;
			inline const Vector2f& GetTileSize() const;

			void AHALA();

			inline bool IsIsometricModeEnabled() const;

			// Load
			bool LoadFromFile(const String& filePath, const TileMapParams& params = TileMapParams());
			bool LoadFromMemory(const void* data, std::size_t size, const TileMapParams& params = TileMapParams());
			bool LoadFromStream(Stream& stream, const TileMapParams& params = TileMapParams());

			inline void SetMaterial(std::size_t layer, std::size_t index, MaterialRef material);

			inline TileMap& operator=(const TileMap& TileMap);
			TileMap& operator=(TileMap&& TileMap) = delete;

			template<typename... Args> static TileMapRef New(Args&&... args);

			struct Tile
			{
				std::size_t index = 0U;
				Color color = Color::White;
				Rectf textureCoords = Rectf::Zero();
				bool enabled = false;
			};

		private:
			Tile& GetTile(std::size_t layer, const Vector2ui& tilePos, std::size_t materialIndex);
			const Tile& GetTile(std::size_t layer, const Vector2ui& tilePos, std::size_t materialIndex) const;
			std::size_t GetTileOffset(const Vector2ui& tilePos) const;
			std::size_t GetLayer(std::size_t layer, std::size_t materialIndex) const;
			void MakeBoundingVolume() const override;
			void UpdateData(InstanceData* instanceData) const override;

			static bool Initialize();
			static void Uninitialize();

			struct Layer
			{
				MaterialRef material;
				std::set<std::size_t> tiles;
			};

			std::vector<Tile> m_tiles;
			std::vector<Layer> m_layers;
			Vector2ui m_mapSize;
			Vector2f m_tileSize;
			std::size_t m_materialCount;
			bool m_isometricModeEnabled;

			static TileMapLibrary::LibraryMap s_library;
			static TileMapLoader::LoaderList s_loaders;
	};
}

#include <Nazara/Graphics/TileMap.inl>

#endif // NAZARA_TILEMAP_HPP
