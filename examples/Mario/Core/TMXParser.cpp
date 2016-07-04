#include "TMXParser.hpp"

#include <Nazara/Core/Log.hpp>

#include "Map.hpp"
#include "LevelData.hpp"

#include "pugixml.hpp"

namespace SMB
{
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

	/*bool TMXParser::LoadFromFile(Nz::String& filename, LevelData& parsingResult)
	{
		pugi::xml_document doc;

		pugi::xml_parse_result result = doc.load_file(filename.GetConstBuffer());

		if (!result)
		{
			NazaraError("XML [" + filename + "] could not be opened" + "] due to: " + result.description());
			return false;
		}

		auto mapNode = doc.child("map");

		Nz::Vector2ui tileDimensions = ParseTile(mapNode, parsingResult);

		Nz::Vector2i dimensions = ParseImage(mapNode.child("tileset"), parsingResult);

		ParseWorld(mapNode, parsingResult);

		return true;
	}

	Nz::Vector2i TMXParser::ParseImage(const pugi::xml_node& tilesetNode, LevelData& result)
	{
		auto imageNode = tilesetNode.child("image");
		result.imageName = imageNode.attribute("source").value();
		auto width = std::stoi(imageNode.attribute("width").value());
		auto height = std::stoi(imageNode.attribute("height").value());
		auto imageDimensions = Nz::Vector2i{ width, height };
		return imageDimensions;
	}

	Nz::Vector2ui TMXParser::ParseTile(const pugi::xml_node& mapNode, LevelData& result)
	{
		auto tilesetNode = mapNode.child("tileset");
		auto tileWidth = std::stoi(tilesetNode.attribute("tilewidth").value());
		auto tileHeight = std::stoi(tilesetNode.attribute("tileheight").value());
		auto tileDimensions = Nz::Vector2ui(tileWidth, tileHeight);
		result.tile = tileDimensions;
		return tileDimensions;
	}

	void TMXParser::ParseWorld(const pugi::xml_node& mapNode, LevelData& result)
	{
		auto layerNode = mapNode.child("layer");

		auto mapDimensionWidth = std::stoi(layerNode.attribute("width").value());
		auto mapDimensionHeight = std::stoi(layerNode.attribute("height").value());

		Map map{ mapDimensionWidth, mapDimensionHeight };
		auto data = layerNode.child("data");
		auto i = 0;
		auto j = 0;
		for (auto tile : data.children())
		{
			map(i, j) = std::stoi(tile.attribute("gid").value());
			i++;

			if (i % mapDimensionWidth == 0)
			{
				i = 0;
				j += 1;
			}
		}

		result.map = std::move(map);
	}*/
}
