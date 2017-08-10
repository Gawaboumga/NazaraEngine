// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Graphics module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Graphics/TileMap.hpp>
#include <Nazara/Graphics/AbstractRenderQueue.hpp>
#include <Nazara/Graphics/AbstractViewer.hpp>
#include <Nazara/Math/Rect.hpp>
#include <cstring>
#include <memory>
#include <Nazara/Graphics/Debug.hpp>

namespace Nz
{
	/*!
	* \ingroup graphics
	* \class Nz::TileMap
	* \brief Graphics class that represent several tiles of the same size assembled into a grid
	*  This class is far more efficient than using a sprite for every tile
	*/

	/*!
	* \brief Constructs a TileMapParams object by default
	*/

	TileMapParams::TileMapParams() :
	loadImage([this](const String& name) -> MaterialRef {
		MaterialRef material = Material::New();
		material->LoadFromFile(pathRoot + name);
		return material;
	}),
	loadTSX([this](const String& name) -> std::unique_ptr<Stream> {
		String path = File::NormalizePath(pathRoot + name);
		std::unique_ptr<File> file = std::make_unique<File>();
		if (!file->Open(path, OpenMode_ReadOnly))
		{
			NazaraError("Could not open file: " + name);
			return nullptr;
		}
		return file;
	})
	{
	}

	/*!
	* \brief Checks whether the parameters for the tile map are correct
	* \return true If parameters are valid
	*/

	bool TileMapParams::IsValid() const
	{
		return true;
	}

	/*!
	* \brief Adds the TileMap to the rendering queue
	*
	* \param renderQueue Queue to be added
	* \param instanceData Data for the instance
	*/

	void TileMap::AddToRenderQueue(AbstractRenderQueue* renderQueue, const InstanceData& instanceData) const
	{
		const VertexStruct_XYZ_Color_UV* vertices = reinterpret_cast<const VertexStruct_XYZ_Color_UV*>(instanceData.data.data());

		std::size_t spriteCount = 0;
		for (const Layer& layer : m_layers)
		{
			if (!layer.tiles.empty() && layer.material)
				renderQueue->AddSprites(instanceData.renderOrder, layer.material, &vertices[4 * spriteCount], layer.tiles.size());
			spriteCount += layer.tiles.size();
		}
	}

	/*!
	* \brief Loads the tile map from file
	* \return true if loading is successful
	*
	* \param filePath Path to the file
	* \param params Parameters for the tile map
	*/

	bool TileMap::LoadFromFile(const String& filePath, const TileMapParams& params)
	{
		if (params.pathRoot.IsEmpty())
		{
			TileMapParams localParams;
			localParams.pathRoot = File::GetDirectory(filePath);
			return TileMapLoader::LoadFromFile(this, filePath, localParams);
		}
		else
			return TileMapLoader::LoadFromFile(this, filePath, params);
	}

	/*!
	* \brief Loads the tile map from memory
	* \return true if loading is successful
	*
	* \param data Raw memory
	* \param size Size of the memory
	* \param params Parameters for the tile map
	*/

	bool TileMap::LoadFromMemory(const void* data, std::size_t size, const TileMapParams& params)
	{
		return TileMapLoader::LoadFromMemory(this, data, size, params);
	}

	/*!
	* \brief Loads the tile map from stream
	* \return true if loading is successful
	*
	* \param stream Stream to the tile map
	* \param params Parameters for the tile map
	*/

	bool TileMap::LoadFromStream(Stream& stream, const TileMapParams& params)
	{
		return TileMapLoader::LoadFromStream(this, stream, params);
	}

	/*!
	* \brief Retrieves the tile
	* \return The specified tile
	*
	* \param layer Layer to consider
	* \param tilePos Position of the tile
	* \param materialIndex Material to select 
	*/

	TileMap::Tile& TileMap::GetTile(std::size_t layer, const Vector2ui& tilePos, std::size_t materialIndex)
	{
		return m_tiles[GetLayer(layer, materialIndex) * m_mapSize.x * m_mapSize.y + GetTileOffset(tilePos)];
	}

	/*!
	* \brief Retrieves the tile
	* \return The specified tile
	*
	* \param layer Layer to consider
	* \param tilePos Position of the tile
	* \param materialIndex Material to select 
	*/

	const TileMap::Tile& TileMap::GetTile(std::size_t layer, const Vector2ui& tilePos, std::size_t materialIndex) const
	{
		return m_tiles[GetLayer(layer, materialIndex) * m_mapSize.x * m_mapSize.y + GetTileOffset(tilePos)];
	}

	std::size_t TileMap::GetTileOffset(const Vector2ui& tilePos) const
	{
		return tilePos.y * m_mapSize.x + tilePos.x;
	}

	std::size_t TileMap::GetLayer(std::size_t layer, std::size_t materialIndex) const
	{
		return layer * GetMaterialCount() + materialIndex;
	}

	/*
	* \brief Makes the bounding volume of this tile map
	*/

	void TileMap::MakeBoundingVolume() const
	{
		Vector2f size = GetSize();
		m_boundingVolume.Set(Vector3f(0.f), size.x * Vector3f::Right() + size.y * Vector3f::Down());
	}

	/*!
	* \brief Updates the data of the tile map
	*
	* \param instanceData Data of the instance
	*/

	void TileMap::UpdateData(InstanceData* instanceData) const
	{
		std::size_t spriteCount = 0;
		for (const Layer& layer : m_layers)
			spriteCount += layer.tiles.size();

		instanceData->data.resize(4 * spriteCount * sizeof(VertexStruct_XYZ_Color_UV));
		VertexStruct_XYZ_Color_UV* vertices = reinterpret_cast<VertexStruct_XYZ_Color_UV*>(instanceData->data.data());

		spriteCount = 0;
		std::size_t offset = 0;
		std::size_t layerIndex = 0;
		for (const Layer& layer : m_layers)
		{
			SparsePtr<Color> colorPtr(&vertices[spriteCount].color, sizeof(VertexStruct_XYZ_Color_UV));
			SparsePtr<Vector3f> posPtr(&vertices[spriteCount].position, sizeof(VertexStruct_XYZ_Color_UV));
			SparsePtr<Vector2f> texCoordPtr(&vertices[spriteCount].uv, sizeof(VertexStruct_XYZ_Color_UV));

			for (std::size_t tileIndex : layer.tiles)
			{
				const Tile& tile = m_tiles[offset + tileIndex];
				NazaraAssert(tile.enabled, "Tile specified for rendering is not enabled");

				std::size_t x = tileIndex % m_mapSize.x;
				std::size_t y = tileIndex / m_mapSize.x;

				Vector3f tileLeftCorner;
				if (m_isometricModeEnabled)
					tileLeftCorner.Set(x * m_tileSize.x + m_tileSize.x / 2.f * (y % 2), y / 2.f * -m_tileSize.y, layerIndex * 0.001f);
				else
					tileLeftCorner.Set(x * m_tileSize.x, y * -m_tileSize.y, layerIndex * 0.001f);

				*colorPtr++ = tile.color;
				*posPtr++ = instanceData->transformMatrix.Transform(tileLeftCorner);
				*texCoordPtr++ = tile.textureCoords.GetCorner(RectCorner_LeftTop);

				*colorPtr++ = tile.color;
				*posPtr++ = instanceData->transformMatrix.Transform(tileLeftCorner + m_tileSize.x * Vector3f::Right());
				*texCoordPtr++ = tile.textureCoords.GetCorner(RectCorner_RightTop);

				*colorPtr++ = tile.color;
				*posPtr++ = instanceData->transformMatrix.Transform(tileLeftCorner + m_tileSize.y * Vector3f::Down());
				*texCoordPtr++ = tile.textureCoords.GetCorner(RectCorner_LeftBottom);

				*colorPtr++ = tile.color;
				*posPtr++ = instanceData->transformMatrix.Transform(tileLeftCorner + m_tileSize.x * Vector3f::Right() + m_tileSize.y * Vector3f::Down());
				*texCoordPtr++ = tile.textureCoords.GetCorner(RectCorner_RightBottom);
			}
			spriteCount += 4 * layer.tiles.size();
			offset += m_mapSize.x * m_mapSize.y;
			++layerIndex;
		}
	}

	/*!
	* \brief Initializes the tilemap library
	* \return true If successful
	*
	* \remark Produces a NazaraError if the tilemap library failed to be initialized
	*/

	bool TileMap::Initialize()
	{
		if (!TileMapLibrary::Initialize())
		{
			NazaraError("Failed to initialise library");
			return false;
		}

		return true;
	}

	/*!
	* \brief Uninitializes the tilemap library
	*/

	void TileMap::Uninitialize()
	{
		TileMapLibrary::Uninitialize();
	}

	TileMapLibrary::LibraryMap TileMap::s_library;
	TileMapLoader::LoaderList TileMap::s_loaders;
}
