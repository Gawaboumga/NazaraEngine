// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#include <TMX/Object.hpp>
#include <TMX/Map.hpp>
#include <TMX/Visitor.hpp>

#include <pugixml/src/pugixml.hpp>
#include <sstream>

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
			else if (name == "ellipse")
				ParseEllipse(node, params);
			else if (name == "point")
				points.push_back(ParsePoint(node, params));
			else if (name == "polygon")
				ParsePolygon(node, params);
			else if (name == "polyline")
				ParsePolyline(node, params);
			else if (name == "text")
				ParseText(node, params);
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

	void Object::ParseEllipse(const pugi::xml_node& ellipseNode, const PropertyParams& params)
	{
		int width = ellipseNode.attribute("width").as_int();
		int height = ellipseNode.attribute("height").as_int();

		Ellipse ellipse;
		ellipse.position = ParsePoint(ellipseNode, params);
		ellipse.size.Set(width, height);
		ellipses.push_back(ellipse);
	}

	Nz::Vector2i Object::ParsePoint(const pugi::xml_node& pointNode, const PropertyParams& params)
	{
		int x = pointNode.attribute("x").as_int();
		int y = pointNode.attribute("y").as_int();
		return Nz::Vector2i(x, y);
	}

	void Object::ParsePoints(const pugi::xml_node& pointsNode, std::vector<Nz::Vector2i>& points)
	{
		std::string data = pointsNode.attribute("points").as_string();
		std::stringstream dataStream(data);

		int x;
		int y;
		while (dataStream >> x)
		{
			if (dataStream.peek() == ',')
				dataStream.ignore();
			dataStream >> y;
			points.emplace_back(x, y);
		}
	}

	void Object::ParsePolygon(const pugi::xml_node& polygonNode, const PropertyParams& params)
	{
		Polygon polygon;
		ParsePoints(polygonNode, polygon.points);
		polygons.push_back(polygon);
	}

	void Object::ParsePolyline(const pugi::xml_node& polylineNode, const PropertyParams& params)
	{
		Polyline polyline;
		ParsePoints(polylineNode, polyline.points);
		polylines.push_back(polyline);
	}

	void Object::ParseText(const pugi::xml_node& textNode, const PropertyParams& params)
	{
		Text text;
		text.font = textNode.attribute("fontfamily").as_string("sans-serif");
		text.fontSize = textNode.attribute("pixelsize").as_uint(16);
		text.wrap = textNode.attribute("wrap").as_bool(false);
		text.color = Nz::Color::FromHex(textNode.attribute("color").as_string("#000000"));
		text.bold = textNode.attribute("bold").as_bool(false);
		text.italic = textNode.attribute("italic").as_bool(false);
		text.underline = textNode.attribute("underline").as_bool(false);
		text.strikeout = textNode.attribute("strikeout").as_bool(false);
		text.kerning = textNode.attribute("kerning").as_bool(false);
		text.halign = AlignmentHorizontalFromString(textNode.attribute("halign").as_string());
		text.valign = AlignmentVerticalFromString(textNode.attribute("valign").as_string());

		texts.push_back(text);
	}
}
