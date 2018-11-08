// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#pragma once

#ifndef NAZARA_TMX_OBJECT_HPP
#define NAZARA_TMX_OBJECT_HPP

#include <TMX/Enums.hpp>
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

			struct Ellipse
			{
				Nz::Vector2i position;
				Nz::Vector2i size;
			};

			struct Polygon
			{
				std::vector<Nz::Vector2i> points;
			};

			struct Polyline
			{
				std::vector<Nz::Vector2i> points;
			};

			struct Text
			{
				Nz::String font;
				Nz::String text;
				Nz::Color color;
				AlignmentHorizontal halign;
				AlignmentVertical valign;
				unsigned int fontSize;
				bool bold;
				bool italic;
				bool kerning;
				bool strikeout;
				bool underline;
				bool wrap;
			};

			std::vector<Ellipse> ellipses;
			std::vector<Nz::Vector2i> points;
			std::vector<Polygon> polygons;
			std::vector<Polyline> polylines;
			std::vector<Property> properties;
			std::vector<Text> texts;
			Nz::String name;
			Nz::String type;
			Nz::Vector2i offset;
			Nz::Vector2i position;
			Nz::Vector2ui size;
			float rotation;
			unsigned int gid;
			unsigned int id;
			bool visible;

		private:
			void ParseEllipse(const pugi::xml_node& ellipseNode, const PropertyParams& params);
			Nz::Vector2i ParsePoint(const pugi::xml_node& pointNode, const PropertyParams& params);
			void ParsePoints(const pugi::xml_node& pointNode, std::vector<Nz::Vector2i>& points);
			void ParsePolygon(const pugi::xml_node& polygonNode, const PropertyParams& params);
			void ParsePolyline(const pugi::xml_node& polylineNode, const PropertyParams& params);
			void ParseText(const pugi::xml_node& textNode, const PropertyParams& params);
	};
}

#endif // NAZARA_TMX_OBJECT_HPP
