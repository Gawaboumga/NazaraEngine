// Copyright (C) 2015 Jérôme Leclercq
// This file is part of the "Nazara Engine".
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_CONTEXTIMPL_HPP
#define NAZARA_CONTEXTIMPL_HPP

#include <Nazara/Renderer/ContextParameters.hpp>
#include <Nazara/Renderer/OpenGL.hpp>

class NzContextImpl
{
	public:
		NzContextImpl();
		~NzContextImpl();

		bool Activate();
		bool Create(NzContextParameters& parameters);
		void Destroy();
		void SwapBuffers();

		static bool Desactivate();

	private:
		GLX::GLXContext m_context;
		GLX::Window m_window;
		GLX::Colormap m_colormap;
		bool m_ownsWindow;
};

#endif // NAZARA_CONTEXTIMPL_HPP
