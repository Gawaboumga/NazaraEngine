// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#include <TMX/TileLayer.hpp>
#include <TMX/Algorithm.hpp>
#include <TMX/Map.hpp>
#include <TMX/Visitor.hpp>

#include <Nazara/Core/ByteArray.hpp>

#include <pugixml/src/pugixml.hpp>
#include <cstring>
#include <sstream>

namespace tmx
{
	TileLayer::TileLayer(const pugi::xml_node& tileLayerNode, const MapParams& params)
	{
		id = tileLayerNode.attribute("id").as_uint();
		name = tileLayerNode.attribute("name").as_string();

		int x = tileLayerNode.attribute("x").as_int();
		int y = tileLayerNode.attribute("y").as_int();
		position.Set(x, y);

		unsigned int width = tileLayerNode.attribute("width").as_int();
		unsigned int height = tileLayerNode.attribute("height").as_int();
		size.Set(width, height);

		tileIds.reserve(width * height);

		opacity = tileLayerNode.attribute("opacity").as_float(1.f);
		visible = tileLayerNode.attribute("visible").as_bool(true);

		int offsetX = tileLayerNode.attribute("offsetx").as_int();
		int offsetY = tileLayerNode.attribute("offsety").as_int();
		offset.Set(offsetX, offsetY);

		for (const auto& node : tileLayerNode.children())
		{
			Nz::String name = node.name();
			if (name == "data")
			{
				Nz::String encoding = node.attribute("encoding").as_string();
				if (encoding == "base64")
					ParseBase64(node);
				else if (encoding == "csv")
					ParseCSV(node);
				else
					ParseUnencoded(node);
			}
			else if (name == "properties")
				properties.emplace_back(node, params.propertyParams);
			else
				NazaraError("Unidentified node: " + name);
		}
	}

	void TileLayer::Accept(Visitor& visitor) const
	{
		visitor.Enter(*this);

		for (const Property& property : properties)
			property.Accept(visitor);

		visitor.Exit(*this);
	}

	void TileLayer::ParseBase64(const pugi::xml_node& node)
	{
		const char* data = node.text().get();
		// We trim left
		while (*data != '\0' && (*data == ' ' || *data == '\r' || *data == '\n'))
			++data;

		Nz::ByteArray byteArray(data, std::strlen(data));

		// We trim right
		int to_remove = 0;
		int last_index = byteArray.GetSize();
		char character = byteArray.GetConstBuffer()[last_index - 1];
		while (last_index != 0 && character == ' ' || character == '\r' || character == '\n')
		{
			--last_index;
			character = byteArray.GetConstBuffer()[last_index - 1];
		}
		byteArray.Resize(last_index);
		if (byteArray.IsEmpty())
		{
			NazaraError("Invalid byte array");
			return;
		}

		byteArray = Base64Decode(byteArray);

		if (node.attribute("compression"))
		{
			Nz::ByteArray decoded(size.x * size.y * 4, '\0');
			Decompress(byteArray, decoded);
			byteArray = decoded;
		}

		const unsigned int* tiles = reinterpret_cast<const unsigned int*>(byteArray.GetConstBuffer());
		int tileCount = byteArray.GetSize() / (sizeof(unsigned int) / sizeof(char));

		for (int i = 0; i != tileCount; ++i)
			tileIds.push_back(tiles[i]);
	}

	void TileLayer::ParseCSV(const pugi::xml_node& node)
	{
		std::string data = node.text().as_string();
		std::stringstream dataStream(data);

		unsigned int i;
		while (dataStream >> i)
		{
			tileIds.push_back(i);
			//TODO this shouldn't assume the first character
			//is a valid value, and it should ignore anything non-numeric.
			if (dataStream.peek() == ',')
				dataStream.ignore();
		}
	}

	void TileLayer::ParseUnencoded(const pugi::xml_node& node)
	{
		Nz::String name;
		for (const auto& child : node.children())
		{
			name = child.name();
			if (name == "tile")
				tileIds.push_back(child.attribute("gid").as_uint());
			else
				NazaraError("Unidentified node: " + name);
		}
	}
}
