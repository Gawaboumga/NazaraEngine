// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Development Kit"
// For conditions of distribution and use, see copyright notice in Prerequesites.hpp

#pragma once

#ifndef NDK_ENUMS_HPP
#define NDK_ENUMS_HPP

#include <Nazara/Core/Flags.hpp>

namespace Ndk
{
	using Nz::EnumAsFlags;

	enum DebugMode
	{
		DebugMode_Collision2D,
		DebugMode_Collision3D,
		DebugMode_Graphics,
		DebugMode_Light,
		DebugMode_Listener,
		DebugMode_Node,
		DebugMode_ParticleEmitter,
		DebugMode_ParticleGroup,
		DebugMode_Physics2D,
		DebugMode_Physics3D,
		DebugMode_Velocity,

		DebugMode_Max = DebugMode_Velocity
	};
}

namespace Nz
{
	template<>
	struct EnumAsFlags<Ndk::DebugMode>
	{
		static constexpr bool value = true;
		static constexpr int  max = Ndk::DebugMode_Max;
	};

	using DebugModeFlags = Flags<Ndk::DebugMode>;
}

namespace Ndk
{
	using Nz::DebugModeFlags;

	constexpr DebugModeFlags DebugMode_All = DebugMode_Collision2D | DebugMode_Collision3D | DebugMode_Graphics | DebugMode_Light |
	DebugMode_Listener | DebugMode_Node | DebugMode_ParticleEmitter | DebugMode_ParticleGroup | DebugMode_Physics2D | DebugMode_Physics3D | DebugMode_Velocity;
}

#endif // NDK_ENUMS_HPP