// Copyright (C) 2015 Jérôme Leclercq
// This file is part of the "Nazara Engine - Graphics module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <memory>
#include <Nazara/Renderer/Debug.hpp>

namespace Nz
{
	/*!
	* \brief Gets the ambient factor
	* \return Current ambient factor
	*/

	inline float Light::GetAmbientFactor() const
	{
		return m_ambientFactor;
	}

	/*!
	* \brief Gets the light attenuation (in 1 / R^2)
	* \return Attenuation
	*/

	inline float Light::GetAttenuation() const
	{
		return m_attenuation;
	}

	/*!
	* \brief Gets the color of the light
	* \return Light color
	*/

	inline Color Light::GetColor() const
	{
		return m_color;
	}

	/*!
	* \brief Gets the diffuse factor
	* \return Current diffuse factor
	*/

	inline float Light::GetDiffuseFactor() const
	{
		return m_diffuseFactor;
	}

	/*!
	* \brief Gets the inner angle in spot light
	* \return Inner angle
	*/

	inline float Light::GetInnerAngle() const
	{
		return m_innerAngle;
	}

	/*!
	* \brief Gets the cosine inner angle in spot light
	* \return Cosine inner angle
	*/

	inline float Light::GetInnerAngleCosine() const
	{
		return m_innerAngleCosine;
	}

	/*!
	* \brief Gets the inverse of the radius
	* \return Inverse of the radius
	*/

	inline float Light::GetInvRadius() const
	{
		return m_invRadius;
	}

	/*!
	* \brief Gets the type of the light
	* \return Light type
	*/

	inline LightType Light::GetLightType() const
	{
		return m_type;
	}

	/*!
	* \brief Gets the outer angle in spot light
	* \return Outer angle
	*/

	inline float Light::GetOuterAngle() const
	{
		return m_outerAngle;
	}

	/*!
	* \brief Gets the cosine outer angle in spot light
	* \return Cosine outer angle
	*/

	inline float Light::GetOuterAngleCosine() const
	{
		return m_outerAngleCosine;
	}

	/*!
	* \brief Gets the tangent outer angle in spot light
	* \return Tangent outer angle
	*/

	inline float Light::GetOuterAngleTangent() const
	{
		return m_outerAngleTangent;
	}

	/*!
	* \brief Gets the radius of the light
	* \return Light radius
	*/

	inline float Light::GetRadius() const
	{
		return m_radius;
	}

	/*!
	* \brief Sets the ambient factor
	*
	* \param factor Ambient factor
	*/

	inline void Light::SetAmbientFactor(float factor)
	{
		m_ambientFactor = factor;
	}

	/*!
	* \brief Sets the light attenuation (in 1 / R^2)
	*
	* \param attenuation Light attenuation
	*/

	inline void Light::SetAttenuation(float attenuation)
	{
		m_attenuation = attenuation;
	}

	/*!
	* \brief Sets the color of the light
	*
	* \param color Light color
	*/

	inline void Light::SetColor(const Color& color)
	{
		m_color = color;
	}

	/*!
	* \brief Sets the diffuse factor
	*
	* \param factor Diffuse factor
	*/

	inline void Light::SetDiffuseFactor(float factor)
	{
		m_diffuseFactor = factor;
	}

	/*!
	* \brief Sets the inner angle in spot light
	* \return innerAngle Inner angle
	*/

	inline void Light::SetInnerAngle(float innerAngle)
	{
		m_innerAngle = innerAngle;
		m_innerAngleCosine = std::cos(DegreeToRadian(m_innerAngle));
	}

	/*!
	* \brief Sets the type of light
	*
	* \param type Light type
	*/

	inline void Light::SetLightType(LightType type)
	{
		m_type = type;
	}

	/*!
	* \brief Sets the outer angle in spot light
	* \return outerAngle Outer angle
	*
	* \remark Invalidates the bounding volume
	*/

	inline void Light::SetOuterAngle(float outerAngle)
	{
		m_outerAngle = outerAngle;
		m_outerAngleCosine = std::cos(DegreeToRadian(m_outerAngle));
		m_outerAngleTangent = std::tan(DegreeToRadian(m_outerAngle));

		InvalidateBoundingVolume();
	}

	/*!
	* \brief Sets the radius of the light
	* \return radius Light radius
	*
	* \remark Invalidates the bounding volume
	*/

	inline void Light::SetRadius(float radius)
	{
		m_radius = radius;

		m_invRadius = 1.f / m_radius;

		InvalidateBoundingVolume();
	}
}

#include <Nazara/Renderer/DebugOff.hpp>
