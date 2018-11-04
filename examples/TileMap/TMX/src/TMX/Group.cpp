// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#include <TMX/Group.hpp>
#include <TMX/Map.hpp>
#include <TMX/Visitor.hpp>

#include <pugixml/src/pugixml.hpp>

namespace tmx
{
	Group::Group(const pugi::xml_node& groupNode, const MapParams& params)
	{
		id = groupNode.attribute("id").as_uint();
		name = groupNode.attribute("name").as_string();

		int offsetX = groupNode.attribute("offsetx").as_int();
		int offsetY = groupNode.attribute("offsety").as_int();
		offset.Set(offsetX, offsetY);

		opacity = groupNode.attribute("opacity").as_float(1.f);
		visible = groupNode.attribute("visible").as_bool(true);

		for (const auto& node : groupNode.children())
		{
			Nz::String name = node.name();
			if (name == "groups")
				groups.emplace_back(node, params);
			else if (name == "imagelayer")
				imageLayers.emplace_back(node, params);
			else if (name == "layer")
				tileLayers.emplace_back(node, params);
			else if (name == "objectgroup")
				objectGroups.emplace_back(node, params.propertyParams);
			else if (name == "properties")
				properties.emplace_back(node, params.propertyParams);
			else
				NazaraError("Unidentified node: " + name);
		}
	}

	void Group::Accept(Visitor& visitor) const
	{
		visitor.Enter(*this);

		for (const Group& group : groups)
			group.Accept(visitor);

		for (const ImageLayer& imageLayer : imageLayers)
			imageLayer.Accept(visitor);

		for (const ObjectGroup& objectGroup : objectGroups)
			objectGroup.Accept(visitor);

		for (const TileLayer& tileLayer : tileLayers)
			tileLayer.Accept(visitor);

		for (const Property& property : properties)
			property.Accept(visitor);

		visitor.Exit(*this);
	}
}
