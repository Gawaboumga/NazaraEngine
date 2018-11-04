// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#pragma once

#ifndef NAZARA_TMX_VISITOR_HPP
#define NAZARA_TMX_VISITOR_HPP

namespace tmx
{
	class Group;
	class ImageLayer;
	class Map;
	class Object;
	class ObjectGroup;
	class Property;
	class Terrain;
	class Tile;
	class TileLayer;
	class TileSet;
	class WangSet;

	class Visitor
	{
		public:
			virtual ~Visitor();

			virtual void Enter(const Group& group);
			virtual void Enter(const ImageLayer& imageLayer);
			virtual void Enter(const Map& map);
			virtual void Enter(const Object& object);
			virtual void Enter(const ObjectGroup& objectGroup);
			virtual void Enter(const Property& property);
			virtual void Enter(const Terrain& terrain);
			virtual void Enter(const Tile& tile);
			virtual void Enter(const TileLayer& tileLayer);
			virtual void Enter(const TileSet& tileSet);
			virtual void Enter(const WangSet& wangSet);

			virtual void Exit(const Group& group);
			virtual void Exit(const ImageLayer& imageLayer);
			virtual void Exit(const Map& map);
			virtual void Exit(const Object& object);
			virtual void Exit(const ObjectGroup& objectGroup);
			virtual void Exit(const Property& property);
			virtual void Exit(const Terrain& terrain);
			virtual void Exit(const Tile& tile);
			virtual void Exit(const TileLayer& tileLayer);
			virtual void Exit(const TileSet& tileSet);
			virtual void Exit(const WangSet& wangSet);
	};
}

#endif // NAZARA_TMX_VISITOR_HPP
