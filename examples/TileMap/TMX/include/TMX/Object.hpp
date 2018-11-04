// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#pragma once

#ifndef NAZARA_TMX_OBJECT_HPP
#define NAZARA_TMX_OBJECT_HPP

#include <TMX/Property.hpp>

#include <Nazara/Math/Vector2.hpp>

namespace pugi
{
	class xml_node;
}

namespace tmx
{
	class Object
	{
		public:
			Object(const pugi::xml_node& objectNode, const PropertyParams& params);

			void Accept(Visitor& visitor) const;

			std::vector<Property> properties;
			Nz::String name;
			Nz::String type;
			Nz::Vector2i offset;
			Nz::Vector2i position;
			Nz::Vector2ui size;
			float rotation;
			unsigned int gid;
			unsigned int id;
			bool visible;
	};
}

#endif // NAZARA_TMX_OBJECT_HPP
