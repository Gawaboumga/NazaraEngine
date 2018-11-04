// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#include <TMX/Property.hpp>
#include <TMX/Visitor.hpp>

#include <pugixml/src/pugixml.hpp>

namespace tmx
{
	bool PropertyParams::IsValid() const
	{
		return false;
	}

	Property::Property(const pugi::xml_node& propertyNode, const PropertyParams& params)
	{
		for (const auto& property : propertyNode.children())
		{
			Nz::String name = property.attribute("name").as_string();

			Nz::String type = property.attribute("type").as_string("string");
			if (type == "bool")
			{
				Nz::String value = property.attribute("value").as_string("false");
				parameters.SetParameter(name, value == "true");
			}
			else if (type == "color")
			{
				Nz::String value = property.attribute("value").as_string();
				Nz::Color color = Nz::Color::FromHex(value);
				parameters.SetParameter(name, color);
			}
			else if (type == "float")
			{
				float value = property.attribute("value").as_float();
				parameters.SetParameter(name, value);
			}
			else if (type == "int")
			{
				long long value = property.attribute("value").as_int();
				parameters.SetParameter(name, value);
			}
			else if (type == "string" || type == "file")
			{
				Nz::String value = property.attribute("value").as_string();
				parameters.SetParameter(name, value);
			}
			else
				NazaraError("Unknown identifier for property: " + name + " with type: " + type);
		}
	}

	void Property::Accept(Visitor& visitor) const
	{
		visitor.Enter(*this);
		visitor.Exit(*this);
	}
}
