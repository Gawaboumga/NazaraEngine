// Copyright (C) 2015 Jérôme Leclercq
// This file is part of the "Nazara Engine - Graphics module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Core/Error.hpp>
#include <Nazara/Graphics/Debug.hpp>

namespace Nz
{
	/*!
	* \brief Gets a pointer to an array of same components
	* \return SparsePtr pointing to an array of same components
	*
	* \param component Component to get in the declaration
	*
	* \remark Produces a NazaraError if component is disabled
	*/

	template <typename T>
	SparsePtr<T> ParticleMapper::GetComponentPtr(ParticleComponent component)
	{
		// Then the component that are interesting
		bool enabled;
		ComponentType type;
		unsigned int offset;
		m_declaration->GetComponent(component, &enabled, &type, &offset);

		if (enabled)
		{
			///TODO: Check the ratio between the type of the attribute and the template type ?
			return SparsePtr<T>(m_ptr + offset, m_declaration->GetStride());
		}
		else
		{
			NazaraError("Attribute 0x" + String::Number(component, 16) + " is not enabled");
			return SparsePtr<T>();
		}
	}

	/*!
	* \brief Gets a pointer to an array of same components
	* \return Constant SparsePtr pointing to an array of same components
	*
	* \param component Component to get in the declaration
	*
	* \remark Produces a NazaraError if component is disabled
	*/

	template <typename T>
	SparsePtr<const T> ParticleMapper::GetComponentPtr(ParticleComponent component) const
	{
		// Then the component that are interesting
		bool enabled;
		ComponentType type;
		unsigned int offset;
		m_declaration->GetComponent(component, &enabled, &type, &offset);

		if (enabled)
		{
			///TODO: Check the ratio between the type of the attribute and the template type ?
			return SparsePtr<const T>(m_ptr + offset, m_declaration->GetStride());
		}
		else
		{
			NazaraError("Attribute 0x" + String::Number(component, 16) + " is not enabled");
			return SparsePtr<const T>();
		}
	}
}

#include <Nazara/Graphics/DebugOff.hpp>
