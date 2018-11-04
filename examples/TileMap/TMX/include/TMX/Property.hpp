// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Nazara Engine - TMX Plugin"

#pragma once

#ifndef NAZARA_TMX_PROPERTY_HPP
#define NAZARA_TMX_PROPERTY_HPP

#include <Nazara/Core/ParameterList.hpp>
#include <Nazara/Core/ResourceParameters.hpp>

namespace pugi
{
	class xml_node;
}

namespace tmx
{
	struct PropertyParams : Nz::ResourceParameters
	{
		bool IsValid() const;
	};

	struct MapParams;
	class Visitor;

	class Property
	{
		public:
			Property(const pugi::xml_node& propertyNode, const PropertyParams& params);

			void Accept(Visitor& visitor) const;

			Nz::ParameterList parameters;
	};
}

#endif // NAZARA_TMX_PROPERTY_HPP
