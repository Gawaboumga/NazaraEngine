// Copyright (C)2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#include <TMX/TileSet.hpp>

namespace tmx
{
	template<typename... Args>
	TileSetRef TileSet::New(Args&&... args)
	{
		std::unique_ptr<TileSet> object(new TileSet(std::forward<Args>(args)...));
		object->SetPersistent(false);

		return object.release();
	}
}
