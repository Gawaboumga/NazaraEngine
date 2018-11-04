// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#include <TMX/Algorithm.hpp>

#include <miniz/miniz.h>

namespace tmx
{
	static inline unsigned char Base64Lookup(unsigned char c)
	{
		if (c >= 'A' && c <= 'Z') return c - 'A';
		if (c >= 'a' && c <= 'z') return c - 71;
		if (c >= '0' && c <= '9') return c + 4;
		if (c == '+') return 62;
		if (c == '/') return 63;
		return 255;
	}

	Nz::ByteArray Base64Decode(const Nz::ByteArray& encodedString)
	{
		static const unsigned char base64Chars[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

		auto in_len = encodedString.GetSize();
		int i = 0;
		int j = 0;
		int in_ = 0;
		unsigned char char_array_4[4], char_array_3[3];
		Nz::ByteArray ret(encodedString.GetSize());

		while (in_len-- && (encodedString[in_] != '='))
		{
			char_array_4[i++] = encodedString[in_]; in_++;
			if (i == 4)
			{
				for (i = 0; i < 4; i++)
					char_array_4[i] = Base64Lookup(char_array_4[i]);

				char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
				char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
				char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

				ret.Append(char_array_3, 3);
				i = 0;
			}
		}

		if (i)
		{
			for (j = i; j < 4; j++)
				char_array_4[j] = '\0';
			for (j = 0; j < i; j++)
				char_array_4[j] = Base64Lookup(char_array_4[j]);
				

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);

			if (i - 1 > 0)
				ret.Append(char_array_3, i - 1);
		}

		return ret;
	}

	bool Decompress(const Nz::ByteArray& inputBuffer, Nz::ByteArray& outputBuffer)
	{
		if (inputBuffer.IsEmpty())
		{
			NazaraError("Input buffer is empty, decompression failed.");
			return false;
		}

		z_stream stream;
		stream.zalloc = Z_NULL;
		stream.zfree = Z_NULL;
		stream.opaque = Z_NULL;
		stream.next_in = inputBuffer.GetConstBuffer();
		stream.avail_in = inputBuffer.GetSize();
		stream.next_out = outputBuffer.GetBuffer();
		stream.avail_out = outputBuffer.GetCapacity();

		//we'd prefer to use inflateInit2 but it appears 
		//to be incorrect in miniz. This is fine for zlib
		//compressed data, but gzip compressed streams
		//will fail to inflate. IMO still preferable to
		//trying to build/link zlib
		if (inflateInit(&stream/*, 15 + 32*/) != Z_OK)
		{
			NazaraError("inflate init failed");
			return false;
		}

		int result = 0;
		do
		{
			result = inflate(&stream, Z_SYNC_FLUSH);

			switch (result)
			{
				case Z_NEED_DICT:
				case Z_STREAM_ERROR:
					result = Z_DATA_ERROR;
				case Z_DATA_ERROR:
					NazaraError("If using gzip compression try using zlib instead");
				case Z_MEM_ERROR:
					inflateEnd(&stream);
					NazaraError("Invalid inflation" + Nz::String::Number(result));
					return false;
			}

			if (result != Z_STREAM_END)
			{
				int oldSize = outputBuffer.GetCapacity();
				outputBuffer.Reserve(oldSize * 2);

				stream.next_out = outputBuffer.GetBuffer() + oldSize;
				stream.avail_out = oldSize;

			}
		} while (result != Z_STREAM_END);

		if (stream.avail_in != 0)
		{
			NazaraError("zlib decompression failed, data still available");
			return false;
		}

		inflateEnd(&stream);
		return true;
	}
}
