#ifndef __PARSINGRESULT_HPP__
#define __PARSINGRESULT_HPP__

#include <Nazara/Core/String.hpp>
#include <Nazara/Math/Vector2.hpp>

#include "Map.hpp"

namespace SMB
{
	struct LevelData
	{
        Nz::String imageName;
        Nz::Vector2ui tile;
        Map map;
	};
}

#endif // __PARSINGRESULT_HPP__
