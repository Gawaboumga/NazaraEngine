// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Renderer module"
// For conditions of distribution and use, see copyright notice in Config.hpp

// Code inspiré de NeHe (Lesson1) et de la SFML par Laurent Gomila

#include <Nazara/Renderer/SDL2/ContextImpl.hpp>
#include <Nazara/Core/CallOnExit.hpp>
#include <Nazara/Core/Error.hpp>
#include <Nazara/Core/LockGuard.hpp>
#include <Nazara/Core/Mutex.hpp>
#include <Nazara/Renderer/Context.hpp>
#include <Nazara/Renderer/OpenGL.hpp>
#include <cstring>
#include <Nazara/Renderer/Debug.hpp>

namespace Nz
{
	ContextImpl::ContextImpl() :
	m_window(nullptr),
	m_context(nullptr)
	{
	}

	bool ContextImpl::Activate() const
	{
		return SDL_GL_MakeCurrent(m_window, m_context) == 0;
	}

	bool ContextImpl::Create(ContextParameters& parameters)
	{
		if (parameters.window)
		{
			m_window = static_cast<SDL_Window*>(parameters.window);
			m_ownsWindow = false;
		}
		else
		{
			m_window = SDL_CreateWindow("STATIC",
				SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, // x and y unspecified
				1, 1,
				SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN
			);

			if (!m_window)
			{
				NazaraError(String("Failed to create window") + SDL_GetError());
				return false;
			}

			m_ownsWindow = true;
		}

		// En cas d'exception, la ressource sera quand même libérée
		CallOnExit onExit([this] ()
		{
			Destroy();
		});

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, (parameters.bitsPerPixel >= 8) ? 8 : parameters.bitsPerPixel);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, (parameters.bitsPerPixel >= 8) ? 8 : parameters.bitsPerPixel);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, (parameters.bitsPerPixel >= 8) ? 8 : parameters.bitsPerPixel);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, (parameters.bitsPerPixel == 32) ? 8 : 0);
		// Debug me
		//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, parameters.doubleBuffered);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, parameters.depthBits);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, parameters.stencilBits);

		if (parameters.antialiasingLevel > 0)
		{
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, parameters.antialiasingLevel);
		}

		if (parameters.shareContext)
			SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, parameters.majorVersion);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, parameters.minorVersion);

		if (parameters.majorVersion >= 3)
		{
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, parameters.compatibilityProfile ? SDL_GL_CONTEXT_PROFILE_COMPATIBILITY : SDL_GL_CONTEXT_PROFILE_CORE);

			// The forward compatible contexts are no more used for this reason :
			// http://www.opengl.org/discussion_boards/showthread.php/175052-Forward-compatible-vs-Core-profile
		}

		if (parameters.debugMode)
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

		if (!m_context)
			m_context = SDL_GL_CreateContext(m_window);

		if (!m_context)
		{
			NazaraError(String("Failed to create context: ") + SDL_GetError());
			return false;
		}

		int majorVersion;
		int minorVersion;
		SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &majorVersion);
		SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minorVersion);
		parameters.majorVersion = majorVersion;
		parameters.minorVersion = minorVersion;

		onExit.Reset();

		return true;
	}

	void ContextImpl::Destroy()
	{
		if (m_context)
		{
			if (SDL_GL_GetCurrentContext() == m_context)
				Desactivate();

			SDL_GL_DeleteContext(m_context);
			m_context = nullptr;
		}

		if (m_ownsWindow && m_window)
		{
			SDL_DestroyWindow(m_window);
			m_window = nullptr;
		}
	}

	void ContextImpl::EnableVerticalSync(bool enabled)
	{
		if (SDL_GL_SetSwapInterval(enabled ? 1 : 0) != 0)
			NazaraError(String("Vertical sync not supported") + SDL_GetError());
	}

	void ContextImpl::SwapBuffers()
	{
		SDL_GL_SwapWindow(m_window);
	}

	bool ContextImpl::Desactivate()
	{
		return SDL_GL_MakeCurrent(nullptr, nullptr) == 0;
	}
}
