// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#include <TMX/Wangset.hpp>
#include <TMX/Visitor.hpp>

#include <pugixml/src/pugixml.hpp>

namespace tmx
{
	WangSet::WangSet(const pugi::xml_node& wangSetNode, const TileSetParams& params)
	{
		name = wangSetNode.attribute("name").as_string();
		tile = wangSetNode.attribute("tile").as_uint();

		for (const auto& node : wangSetNode.children())
		{
			Nz::String name = node.name();
			if (name == "wangcornercolor")
				ParseWangCornerColor(node, params);
			else if (name == "wangedgecolor")
				ParseWangEdgeColor(node, params);
			else if (name == "wangtile")
				ParseWangTile(node, params);
			else
				NazaraError("Unidentified node: " + name);
		}
	}

	void WangSet::Accept(Visitor& visitor) const
	{
		visitor.Enter(*this);
		visitor.Exit(*this);
	}

	void WangSet::ParseWangCornerColor(const pugi::xml_node& wangCornerColorNode, const TileSetParams& params)
	{
		WangCornerColor wangCornerColor;
		wangCornerColor.name = wangCornerColorNode.attribute("name").as_string();
		wangCornerColor.color = Nz::Color::FromHex(wangCornerColorNode.attribute("color").as_string());
		wangCornerColor.tile = wangCornerColorNode.attribute("tile").as_uint();
		wangCornerColor.probability = wangCornerColorNode.attribute("probability").as_float();
		wangCornerColors.emplace_back(wangCornerColor);
	}

	void WangSet::ParseWangEdgeColor(const pugi::xml_node& wangEdgeColorNode, const TileSetParams& params)
	{
		WangEdgeColor wangEdgeColor;
		wangEdgeColor.name = wangEdgeColorNode.attribute("name").as_string();
		wangEdgeColor.color = Nz::Color::FromHex(wangEdgeColorNode.attribute("color").as_string());
		wangEdgeColor.tile = wangEdgeColorNode.attribute("tile").as_uint();
		wangEdgeColor.probability = wangEdgeColorNode.attribute("probability").as_float();
		wangEdgeColors.emplace_back(wangEdgeColor);
	}

	void WangSet::ParseWangTile(const pugi::xml_node& wangTileNode, const TileSetParams& params)
	{
		WangTile wangTile;
		wangTile.tileId = wangTileNode.attribute("tileid").as_uint();
		wangTile.wangId = wangTileNode.attribute("wangid").as_uint();
		wangTiles.emplace_back(wangTile);
	}
}
