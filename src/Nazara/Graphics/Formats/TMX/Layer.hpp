// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Graphics module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_LOADERS_TMX_LAYER_HPP
#define NAZARA_LOADERS_TMX_LAYER_HPP

#include <Nazara/Prerequesites.hpp>
#include <Nazara/Graphics/Formats/TMX/Map.hpp>
#include <cppcodec/base64_default_rfc4648.hpp>
#include <cppcodec/base64_rfc4648.hpp>
#include <pugixml/pugixml.hpp>
#include <miniz/miniz.h>

namespace Nz
{
	namespace TMX
	{
		char* Deblank(char* str)
		{
			char* put = str;

			for (; *str != '\0'; ++str)
			{
				if (*str != ' ' && *str != '\r' && *str != '\n')
					*put++ = *str;
			}
			*put = '\0';

			return put;
		}

		int inflate_stream(const Bytef* input, uLong inputLen, Bytef* output, uLong outLen)
		{
			z_stream strm;
			strm.zalloc = Z_NULL;
			strm.zfree = Z_NULL;
			strm.opaque = Z_NULL;
			strm.avail_in = inputLen;
			strm.next_in = input;
			int ret = inflateInit(&strm);
			if (ret != Z_OK)
				return ret;
		
			/* run inflate() on input until output buffer not full */
			do {
				strm.avail_out = outLen;
				strm.next_out = output;
				ret = inflate(&strm, Z_NO_FLUSH);
				assert(ret != Z_STREAM_ERROR); /* state not clobbered */
				switch (ret) {
				case Z_NEED_DICT:
					ret = Z_DATA_ERROR; /* and fall through */
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					(void)inflateEnd(&strm);
					return ret;
				}
			} while (strm.avail_in != 0);

			/* clean up and return */
			(void)inflateEnd(&strm);
			return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
		}

		struct Layer
		{
			Nz::String name;
			float opacity = 1.f;
			bool visible = true;
			std::vector<Int32> ids;

			bool Parse(const pugi::xml_node& layer, const Map& map, const TileMapParams& parameters)
			{
				name = layer.attribute("name").value();
				opacity = layer.attribute("opacity").as_float(1.f);
				visible = layer.attribute("visible").as_bool(true);

				pugi::xml_node data = layer.child("data");

				const char* text = data.text().get();
				const char* endString = Deblank(const_cast<char*>(text));

				std::vector<uint8_t> decoded = base64::decode(text, std::distance(text, endString));

				mz_ulong uncompressedSize = map.mapSize.x * map.mapSize.y;
				ids.resize(uncompressedSize, 0);
				mz_ulong decodedSize = decoded.size();
				int ret = inflate_stream(decoded.data(), decodedSize,
					reinterpret_cast<UInt8*>(ids.data()), uncompressedSize * 4); // This is treated by bytes not ints !
				if (ret != Z_OK)
				{
					NazaraError("Error while decoding data: " + String(mz_error(ret)));
					return false;
				}

				return true;
			}
		};
	}
}

#endif // NAZARA_LOADERS_TMX_LAYER_HPP
