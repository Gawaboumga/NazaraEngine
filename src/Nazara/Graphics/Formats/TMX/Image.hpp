// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Graphics module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_LOADERS_TMX_IMAGE_HPP
#define NAZARA_LOADERS_TMX_IMAGE_HPP

#include <Nazara/Prerequesites.hpp>
#include <Nazara/Utility/Image.hpp>
#include <pugixml/pugixml.hpp>

namespace Nz
{
	namespace TMX
	{
		struct Image
		{
			MaterialRef material;
			Vector2ui imageSize;

			bool Parse(const pugi::xml_node& imageNode, const TileMapParams& parameters)
			{
				String path = imageNode.attribute("source").value();
				material = parameters.loadImage(path);

				imageSize = Vector2ui(imageNode.attribute("width").as_uint(), imageNode.attribute("height").as_uint());
				if (imageSize.x == 0 || imageSize.y == 0)
				{
					const TextureRef& texture = material->GetDiffuseMap();
					if (!texture)
					{
						NazaraError("Invalid image");
						return false;
					}
					imageSize = Vector2ui(texture->GetWidth(), texture->GetHeight());
				}

				return true;
			}
		};
	}
}

#endif // NAZARA_LOADERS_TMX_IMAGE_HPP
