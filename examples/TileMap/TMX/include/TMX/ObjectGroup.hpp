// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#pragma once

#ifndef NAZARA_TMX_OBJECT_GROUP_HPP
#define NAZARA_TMX_OBJECT_GROUP_HPP

#include <TMX/Enums.hpp>
#include <TMX/Object.hpp>
#include <TMX/Property.hpp>

#include <Nazara/Math/Vector2.hpp>

namespace pugi
{
	class xml_node;
}

namespace tmx
{
	struct MapParams;

	class ObjectGroup
	{
		public:
			ObjectGroup(const pugi::xml_node& objectGroupNode, const PropertyParams& params);

			void Accept(Visitor& visitor) const;

			std::vector<Object> objects;
			std::vector<Property> properties;
			Nz::String name;
			Nz::Color color;
			Nz::Vector2i offset;
			Nz::Vector2i position;
			Nz::Vector2ui size;
			float opacity;
			unsigned int id;
			DrawOrder drawOrder;
			bool visible;
	};
}

#endif // NAZARA_TMX_OBJECT_GROUP_HPP
