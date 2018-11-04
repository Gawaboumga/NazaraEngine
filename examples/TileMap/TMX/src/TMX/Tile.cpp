// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#include <TMX/Tile.hpp>
#include <TMX/TileSet.hpp>
#include <TMX/Visitor.hpp>

#include <pugixml/src/pugixml.hpp>

namespace tmx
{
	Tile::Tile(const pugi::xml_node& tileNode, const TileSetParams& params)
	{
		id = tileNode.attribute("id").as_uint();
		type = tileNode.attribute("type").as_string();
		terrain = tileNode.attribute("terrain").as_string();
		probability = tileNode.attribute("probability").as_float();

		for (const auto& node : tileNode.children())
		{
			Nz::String name = node.name();
			if (name == "objectgroups")
				objectGroups.emplace_back(node, params.propertyParams);
			else if (name == "properties")
				properties.emplace_back(node, params.propertyParams);
			else
				NazaraError("Unidentified node: " + name);
		}
	}

	void Tile::Accept(Visitor& visitor) const
	{
		visitor.Enter(*this);

		for (const ObjectGroup& objectGroup : objectGroups)
			objectGroup.Accept(visitor);

		for (const Property& property : properties)
			property.Accept(visitor);

		visitor.Exit(*this);
	}
}
