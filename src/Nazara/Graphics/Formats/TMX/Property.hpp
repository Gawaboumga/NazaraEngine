// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Graphics module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_LOADERS_TMX_PROPERTY_HPP
#define NAZARA_LOADERS_TMX_PROPERTY_HPP

#include <Nazara/Prerequesites.hpp>
#include <pugixml/pugixml.hpp>

namespace Nz
{
	namespace TMX
	{
		struct Property
		{
			ParameterList parameterList;

			bool Parse(const pugi::xml_node& properties, const TileMapParams& parameters)
			{
				for (const pugi::xml_node& property : properties.children("property"))
				{
					Nz::String name = property.attribute("name").value();
					const char* type = property.attribute("type").as_string();
					const pugi::xml_attribute value = property.attribute("value");
					if (std::strcmp(type, "int")
						parameterList.SetParameter(name, value.as_int());
					else if (std::strcmp(type, "float")
						parameterList.SetParameter(name, value.as_float());
					else if (std::strcmp(type, "bool")
						parameterList.SetParameter(name, value.as_bool());
					else if (std::strcmp(type, "color")
						parameterList.SetParameter(name, Color(value.as_string()));
					else
						parameterList.SetParameter(name, value.as_string());
				}

				return true;
			}
		};
	}
}

#endif // NAZARA_LOADERS_TMX_PROPERTY_HPP
