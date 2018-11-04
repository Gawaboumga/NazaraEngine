// Copyright (C)2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#include <TMX/Map.hpp>

namespace tmx
{
	template<typename... Args>
	MapRef Map::New(Args&&... args)
	{
		std::unique_ptr<Map> object(new Map(std::forward<Args>(args)...));
		object->SetPersistent(false);

		return object.release();
	}
}
