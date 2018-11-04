// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#pragma once

#ifndef NAZARA_TMX_GROUP_HPP
#define NAZARA_TMX_GROUP_HPP

#include <TMX/ImageLayer.hpp>
#include <TMX/TileLayer.hpp>
#include <TMX/ObjectGroup.hpp>
#include <TMX/Property.hpp>

#include <Nazara/Math/Vector2.hpp>

namespace pugi
{
	class xml_node;
}

namespace tmx
{
	struct MapParams;
	class Visitor;

	class Group
	{
		public:
			Group(const pugi::xml_node& groupNode, const MapParams& params);

			void Accept(Visitor& visitor) const;

			std::vector<Group> groups;
			std::vector<ImageLayer> imageLayers;
			std::vector<ObjectGroup> objectGroups;
			std::vector<Property> properties;
			std::vector<TileLayer> tileLayers;
			Nz::String name;
			Nz::Vector2i offset;
			float opacity;
			unsigned int id;
			bool visible;
	};
}

#endif // NAZARA_TMX_GROUP_HPP
