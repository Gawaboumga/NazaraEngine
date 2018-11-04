// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#include <TMX/Object.hpp>
#include <TMX/Map.hpp>
#include <TMX/Visitor.hpp>

#include <pugixml/src/pugixml.hpp>

namespace tmx
{
	Object::Object(const pugi::xml_node& objectNode, const PropertyParams& params)
	{
		id = objectNode.attribute("id").as_uint();
		name = objectNode.attribute("name").as_string();
		type = objectNode.attribute("type").as_string();

		int x = objectNode.attribute("x").as_int();
		int y = objectNode.attribute("y").as_int();
		position.Set(x, y);

		unsigned int width = objectNode.attribute("width").as_int();
		unsigned int height = objectNode.attribute("height").as_int();
		size.Set(width, height);

		rotation = objectNode.attribute("rotation").as_float();
		gid = objectNode.attribute("gid").as_uint();
		visible = objectNode.attribute("visible").as_bool(true);

		for (const auto& node : objectNode.children())
		{
			Nz::String name = node.name();
			if (name == "properties")
				properties.emplace_back(node, params);
			else
				NazaraError("Unidentified node: " + name);
		}
	}

	void Object::Accept(Visitor& visitor) const
	{
		visitor.Enter(*this);

		for (const Property& property : properties)
			property.Accept(visitor);

		visitor.Exit(*this);
	}
}
