// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#pragma once

#ifndef NAZARA_TMX_ALGORITHM_HPP
#define NAZARA_TMX_ALGORITHM_HPP

#include <Nazara/Core/ByteArray.hpp>

namespace tmx
{
	Nz::ByteArray Base64Decode(const Nz::ByteArray& encodedString);

	bool Decompress(const Nz::ByteArray& inputBuffer, Nz::ByteArray& outputBuffer);
}

#endif // NAZARA_TMX_ALGORITHM_HPP
