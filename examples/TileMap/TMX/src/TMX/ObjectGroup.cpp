// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#include <TMX/ObjectGroup.hpp>
#include <TMX/Enums.hpp>
#include <TMX/Map.hpp>
#include <TMX/Visitor.hpp>

#include <pugixml/src/pugixml.hpp>

namespace tmx
{
	ObjectGroup::ObjectGroup(const pugi::xml_node& objectGroupNode, const PropertyParams& params)
	{
		id = objectGroupNode.attribute("id").as_uint();
		name = objectGroupNode.attribute("name").as_string();
		Nz::String colorString = objectGroupNode.attribute("color").as_string("#000000");
		color = Nz::Color::FromHex(colorString);

		int x = objectGroupNode.attribute("x").as_int();
		int y = objectGroupNode.attribute("y").as_int();
		position.Set(x, y);

		unsigned int width = objectGroupNode.attribute("width").as_int();
		unsigned int height = objectGroupNode.attribute("height").as_int();
		size.Set(width, height);

		opacity = objectGroupNode.attribute("opacity").as_float(1.f);
		visible = objectGroupNode.attribute("visible").as_bool(true);

		int offsetX = objectGroupNode.attribute("offsetx").as_int();
		int offsetY = objectGroupNode.attribute("offsety").as_int();
		offset.Set(offsetX, offsetY);

		Nz::String drawOrderString = objectGroupNode.attribute("draworder").as_string("topdown");
		drawOrder = DrawOrderFromString(drawOrderString);

		for (const auto& node : objectGroupNode.children())
		{
			Nz::String name = node.name();
			if (name == "object")
				objects.emplace_back(node, params);
			else if (name == "properties")
				properties.emplace_back(node, params);
			else
				NazaraError("Unidentified node: " + name);
		}
	}

	void ObjectGroup::Accept(Visitor& visitor) const
	{
		visitor.Enter(*this);

		for (const Object& object : objects)
			object.Accept(visitor);

		for (const Property& property : properties)
			property.Accept(visitor);

		visitor.Exit(*this);
	}
}
