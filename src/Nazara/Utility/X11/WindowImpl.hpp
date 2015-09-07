// Copyright (C) 2015 Jérôme Leclercq
// This file is part of the "Nazara Engine - Utility module"
// For conditions of distribution and use, see copyright notice in Config.hpp

// Interface inspirée de la SFML par Laurent Gomila

#pragma once

#ifndef NAZARA_WINDOWIMPL_HPP
#define NAZARA_WINDOWIMPL_HPP

#include <Nazara/Core/NonCopyable.hpp>
#include <Nazara/Core/String.hpp>
#include <Nazara/Core/Thread.hpp>
#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Utility/Config.hpp>
#include <Nazara/Utility/Keyboard.hpp>
#include <Nazara/Utility/Mouse.hpp>
#include <Nazara/Utility/VideoMode.hpp>
#include <Nazara/Utility/Window.hpp>
#include <GL/glx.h>
#include <X11/Xlib-xcb.h>
#include <xcb/randr.h>
#include <xcb/xcb_icccm.h>
#include <Nazara/Utility/X11/Display.hpp>

#if NAZARA_UTILITY_THREADED_WINDOW
class NzConditionVariable;
class NzMutex;
#endif
class NzWindow;

class NzWindowImpl : NzNonCopyable
{
	public:
		NzWindowImpl(NzWindow* parent);
		~NzWindowImpl();

		bool Create(const NzVideoMode& mode, const NzString& title, nzUInt32 style);
		bool Create(NzWindowHandle handle);

		void Destroy();

		void EnableKeyRepeat(bool enable);
		void EnableSmoothScrolling(bool enable);

		NzWindowHandle GetHandle() const;
		unsigned int GetHeight() const;
		NzVector2i GetPosition() const;
		NzVector2ui GetSize() const;
		nzUInt32 GetStyle() const;
		NzString GetTitle() const;
		unsigned int GetWidth() const;

		bool HasFocus() const;

		void IgnoreNextMouseEvent(int mouseX, int mouseY);

		bool IsMinimized() const;
		bool IsVisible() const;

		void ProcessEvents(bool block);

		void SetCursor(nzWindowCursor cursor);
		void SetCursor(const NzCursor& cursor);
		void SetEventListener(bool listener);
		void SetFocus();
		void SetIcon(const NzIcon& icon);
		void SetMaximumSize(int width, int height);
		void SetMinimumSize(int width, int height);
		void SetPosition(int x, int y);
		void SetSize(unsigned int width, unsigned int height);
		void SetStayOnTop(bool stayOnTop);
		void SetTitle(const NzString& title);
		void SetVisible(bool visible);

		static bool Initialize();
		static void Uninitialize();

	private:

		void SetVideoMode(const NzVideoMode& mode);

		XVisualInfo SelectBestVisual(nzUInt8 bitsPerPixel, nzUInt32 style);
		void SwitchToFullscreen();

		void ResetVideoMode();

		void CommonInitialize();
		void CleanUp();
		void CreateHiddenCursor();

		bool ProcessEvent(xcb_generic_event_t* windowEvent);

		NzKeyboard::Key ConvertVirtualKey(KeySym symbol);
		xcb_keysym_t ConvertKeyCodeToKeySym(xcb_keycode_t keycode, uint16_t state);
		const char* ConvertWindowCursorToXName(nzWindowCursor cursor);
		void SetCursor(xcb_cursor_t cursor);
		void SetMotifHints();

		xcb_window_t                      m_window;
		xcb_connection_t*                 m_connection;
		xcb_screen_t*                     m_screen;
		xcb_randr_get_screen_info_reply_t m_oldVideoMode;
		xcb_cursor_t                      m_hiddenCursor;
		xcb_size_hints_t m_size_hints;
		nzUInt32 m_style;
		#if NAZARA_UTILITY_THREADED_WINDOW
		NzThread m_thread;
		#endif
		NzWindow* m_parent;
		bool m_eventListener;
		bool m_ownsWindow;
		#if !NAZARA_UTILITY_THREADED_WINDOW
		bool m_sizemove;
		#endif
		bool m_smoothScrolling;
		#if NAZARA_UTILITY_THREADED_WINDOW
		bool m_threadActive;
		#endif
		short m_scrolling;
		NzVector2i m_mousePos;
		bool m_keyRepeat;

		void UpdateEventQueue(xcb_generic_event_t* event);

		struct
		{
			xcb_generic_event_t* curr = nullptr;
			xcb_generic_event_t* next = nullptr;
		} m_eventQueue;
};
#endif // NAZARA_WINDOWIMPL_HPP
