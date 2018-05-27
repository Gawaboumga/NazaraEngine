// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Platform module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Platform/Joystick.hpp>
#include <Nazara/Platform/SDL2/JoystickImpl.hpp>
#include <Nazara/Platform/Debug.hpp>

namespace Nz
{
	Joystick::~Joystick()
	{
		delete m_impl;
		m_impl = nullptr;
	}

	short Joystick::GetAxis(unsigned int axis) const
	{
		#if NAZARA_PLATFORM_SAFE
		if (!IsValid())
		{
			NazaraError("Joystick invalid");
			return 0;
		}
		#endif
		return m_impl->GetAxis(axis);
	}

	unsigned int Joystick::GetAxisCount() const
	{
		#if NAZARA_PLATFORM_SAFE
		if (!IsValid())
		{
			NazaraError("Joystick invalid");
			return 0;
		}
		#endif
		return m_impl->GetAxisCount();
	}

	Vector2i Joystick::GetBall(unsigned int ball) const
	{
		#if NAZARA_PLATFORM_SAFE
		if (!IsValid())
		{
			NazaraError("Joystick invalid");
			return Vector2i::Zero();
		}
		#endif
		return m_impl->GetBall(ball);
	}

	unsigned int Joystick::GetBallCount() const
	{
		#if NAZARA_PLATFORM_SAFE
		if (!IsValid())
		{
			NazaraError("Joystick invalid");
			return 0;
		}
		#endif
		return m_impl->GetBallCount();
	}

	JoystickHat Joystick::GetHat(unsigned int hat) const
	{
		#if NAZARA_PLATFORM_SAFE
		if (!IsValid())
		{
			NazaraError("Joystick invalid");
			return JoystickHat_Centered;
		}
		#endif
		return m_impl->GetHat(hat);
	}

	unsigned int Joystick::GetHatCount() const
	{
		#if NAZARA_PLATFORM_SAFE
		if (!IsValid())
		{
			NazaraError("Joystick invalid");
			return 0;
		}
		#endif
		return m_impl->GetHatCount();
	}

	unsigned int Joystick::GetId() const
	{
		#if NAZARA_PLATFORM_SAFE
		if (!IsValid())
		{
			NazaraError("Joystick invalid");
			return -1;
		}
		#endif
		return m_impl->GetId();
	}

	String Joystick::GetName() const
	{
		#if NAZARA_PLATFORM_SAFE
		if (!IsValid())
		{
			NazaraError("Joystick invalid");
			return false;
		}
		#endif
		return m_impl->GetName();
	}

	bool Joystick::IsAttached() const
	{
		#if NAZARA_PLATFORM_SAFE
		if (!IsValid())
		{
			NazaraError("Joystick invalid");
			return false;
		}
		#endif
		return m_impl->IsAttached();
	}

	bool Joystick::IsButtonPressed(unsigned int button) const
	{
		#if NAZARA_PLATFORM_SAFE
		if (!IsValid())
		{
			NazaraError("Joystick invalid");
			return false;
		}
		#endif
		return m_impl->IsButtonPressed(button);
	}

	bool Joystick::IsValid() const
	{
		return m_impl->IsValid();
	}

	Joystick Joystick::Get(unsigned int joystickId)
	{
		return Joystick(joystickId);
	}

	Joystick Joystick::Get(const String& name)
	{
		for (unsigned int joystickId = 0; joystickId != GetJoystickCount(); ++joystickId)
		{
			Joystick joystick(joystickId);
			if (joystick.IsValid() && joystick.GetName() == name)
				return joystick;
		}

		return Joystick(-1);
	}

	unsigned int Joystick::GetJoystickCount()
	{
		return JoystickImpl::GetJoystickCount();
	}

	Joystick::Joystick(unsigned int joystickId)
	{
		m_impl = new JoystickImpl(joystickId);
	}

	bool Joystick::Initialize()
	{
		return JoystickImpl::Initialize();
	}

	void Joystick::Uninitialize()
	{
		return JoystickImpl::Uninitialize();
	}
}
