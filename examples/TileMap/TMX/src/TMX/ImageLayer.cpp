// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#include <TMX/ImageLayer.hpp>
#include <TMX/Map.hpp>
#include <TMX/Visitor.hpp>

#include <pugixml/src/pugixml.hpp>

namespace tmx
{
	ImageLayer::ImageLayer(const pugi::xml_node& imageLayerNode, const MapParams& params)
	{
		id = imageLayerNode.attribute("id").as_uint();
		name = imageLayerNode.attribute("name").as_string();

		int offsetX = imageLayerNode.attribute("offsetx").as_int();
		int offsetY = imageLayerNode.attribute("offsety").as_int();
		offset.Set(offsetX, offsetY);

		int x = imageLayerNode.attribute("x").as_int();
		int y = imageLayerNode.attribute("y").as_int();
		position.Set(x, y);

		opacity = imageLayerNode.attribute("opacity").as_float(1.f);
		visible = imageLayerNode.attribute("visible").as_bool(true);

		for (const auto& node : imageLayerNode.children())
		{
			Nz::String name = node.name();
			if (name == "image")
				continue;
			else if (name == "properties")
				properties.emplace_back(node, params.propertyParams);
			else
				NazaraError("Unidentified node: " + name);
		}

		pugi::xml_node imageNode = imageLayerNode.child("image");
		image = Nz::Image::LoadFromFile(imageNode.attribute("source").as_string());
	}

	void ImageLayer::Accept(Visitor& visitor) const
	{
		visitor.Enter(*this);

		for (const Property& property : properties)
			property.Accept(visitor);

		visitor.Exit(*this);
	}
}
