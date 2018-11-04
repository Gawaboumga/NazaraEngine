// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#pragma once

#ifndef NAZARA_TMX_IMAGE_LAYER_HPP
#define NAZARA_TMX_IMAGE_LAYER_HPP

#include <TMX/Property.hpp>

#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Utility/Image.hpp>

namespace pugi
{
	class xml_node;
}

namespace tmx
{
	struct MapParams;

	class ImageLayer
	{
		public:
			ImageLayer(const pugi::xml_node& imageLayerNode, const MapParams& params);

			void Accept(Visitor& visitor) const;

			std::vector<Property> properties;
			Nz::ImageRef image;
			Nz::String name;
			Nz::Vector2i offset;
			Nz::Vector2i position;
			float opacity;
			unsigned int id;
			bool visible;
	};
}

#endif // NAZARA_TMX_IMAGE_LAYER_HPP
