// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#include <TMX/Terrain.hpp>
#include <TMX/TileSet.hpp>
#include <TMX/Visitor.hpp>

#include <pugixml/src/pugixml.hpp>

namespace tmx
{
	Terrain::Terrain(const pugi::xml_node& terrainNode, const TileSetParams& params)
	{
		name = terrainNode.attribute("name").as_string();
		tile = terrainNode.attribute("tile").as_uint();

		for (const auto& node : terrainNode.children())
		{
			Nz::String name = node.name();
			if (name == "properties")
				properties.emplace_back(node, params.propertyParams);
			else
				NazaraError("Unidentified node: " + name);
		}
	}

	void Terrain::Accept(Visitor& visitor) const
	{
		visitor.Enter(*this);

		for (const Property& property : properties)
			property.Accept(visitor);

		visitor.Exit(*this);
	}
}
