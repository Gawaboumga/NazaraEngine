#include "TMXParser.hpp"

#include <Nazara/Core/Log.hpp>

#include "Map.hpp"
#include "LevelData.hpp"

#include "pugixml.hpp"

namespace SMB
{
	std::vector<SMB::Enemy> TMXParser::GetEnemies(const Nz::String& filename)
	{
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(filename.GetConstBuffer());
		if (!result)
		{
			NazaraError("XML [" + filename + "] could not be opened" + "] due to: " + result.description());
			return {};
		}

		std::vector<Enemy> enemies{};
		auto enemiesNode = doc.child("enemies");
		for (auto enemyNode : enemiesNode.children())
		{
			Enemy enemy {
				std::stoi(enemyNode.attribute("x").value()),
				std::stoi(enemyNode.attribute("y").value()),
				Enemy::FromString(enemyNode.attribute("type").value())
			};
			enemies.push_back(std::move(enemy));
		}
		return enemies;
	}

	Matrix<Nz::String, 2> TMXParser::GetMap(const Nz::String& filename)
	{
		pugi::xml_document doc;
		pugi::xml_node mapNode = GetMapNode(doc, filename);
		auto layerNode = mapNode.child("layer");

		auto mapDimensionWidth = std::stoi(layerNode.attribute("width").value());
		auto mapDimensionHeight = std::stoi(layerNode.attribute("height").value());

		Matrix<Nz::String, 2> map{{ mapDimensionWidth, mapDimensionHeight }};
		auto data = layerNode.child("data");
		auto i = 0;
		auto j = 0;
		for (auto tile : data.children())
		{
			map({i, j}) = tile.attribute("gid").value();
			i++;

			if (i % mapDimensionWidth == 0)
			{
				i = 0;
				j += 1;
			}
		}

		return map;
	}

	Nz::Vector2ui TMXParser::GetTileSize(const Nz::String& filename)
	{
		pugi::xml_document doc;
		pugi::xml_node mapNode = GetMapNode(doc, filename);

		auto tilesetNode = mapNode.child("tileset");
		auto tileWidth = std::stoi(tilesetNode.attribute("tilewidth").value());
		auto tileHeight = std::stoi(tilesetNode.attribute("tileheight").value());
		return Nz::Vector2ui(tileWidth, tileHeight);
	}

	pugi::xml_node TMXParser::GetMapNode(pugi::xml_document& doc, const Nz::String& filename)
	{
		pugi::xml_parse_result result = doc.load_file(filename.GetConstBuffer());
		if (!result)
		{
			NazaraError("XML [" + filename + "] could not be opened" + "] due to: " + result.description());
			return {};
		}

		return doc.child("map");
	}
}
