////////////////////////////////////////////////////////////
//
// Copyright (C) 2014 Maximilian Wagenbach (aka. Foaly) (foaly.f@web.de)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
// you must not claim that you wrote the original software.
// If you use this software in a product, an acknowledgment
// in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
// and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#include "AnimatedSprite.hpp"

#include "Animation.hpp"

namespace SMB
{

	AnimatedSprite::AnimatedSprite(float frameTime, bool paused, bool looped) :
		m_animation(nullptr),
		m_frameTime(frameTime),
		m_currentFrame(0),
		m_isPaused(paused),
		m_isLooped(looped)
	{
	}

	void AnimatedSprite::SetAnimation(const Animation& animation)
	{
		if (m_animation == &animation)
			return;
		m_animation = &animation;
		SetTexture(m_animation->GetSpriteSheet(), false);
		m_currentFrame = 0;
		SetFrame(m_currentFrame);
	}

	void AnimatedSprite::SetFrameTime(float time)
	{
		m_frameTime = time;
	}

	void AnimatedSprite::Play()
	{
		m_isPaused = false;
	}

	void AnimatedSprite::Play(const Animation& animation)
	{
		if (GetAnimation() != &animation)
			SetAnimation(animation);
		Play();
	}

	void AnimatedSprite::Pause()
	{
		m_isPaused = true;
	}

	void AnimatedSprite::Stop()
	{
		m_isPaused = true;
		m_currentFrame = 0;
		SetFrame(m_currentFrame);
	}

	void AnimatedSprite::SetLooped(bool looped)
	{
		m_isLooped = looped;
	}

	const Animation* AnimatedSprite::GetAnimation() const
	{
		return m_animation;
	}

	bool AnimatedSprite::IsLooped() const
	{
		return m_isLooped;
	}

	bool AnimatedSprite::IsPlaying() const
	{
		return !m_isPaused;
	}

	float AnimatedSprite::GetFrameTime() const
	{
		return m_frameTime;
	}

	void AnimatedSprite::SetFrame(std::size_t newFrame, bool resetTime)
	{
		if (m_animation)
		{
			Nz::Rectui rect = m_animation->GetFrame(newFrame);
			SetTextureRect(rect);
		}

		if (resetTime)
			m_currentTime = 0.f;
	}

	void AnimatedSprite::Update(float deltaTime)
	{
		// if not paused and we have a valid animation
		if (!m_isPaused && m_animation)
		{
			// add delta time
			m_currentTime += deltaTime;

			// if current time is bigger then the frame time advance one frame
			if (m_currentTime >= m_frameTime)
			{
				// reset time, but keep the remainder
				m_currentTime = std::fmod(m_currentTime, m_frameTime);

				// get next Frame index
				if (m_currentFrame + 1 < m_animation->GetSize())
					m_currentFrame++;
				else
				{
					// animation has ended
					m_currentFrame = 0; // reset to start

					if (!m_isLooped)
					{
						m_isPaused = true;
					}

				}

				// set the current frame, not reseting the time
				SetFrame(m_currentFrame, false);
			}
		}
	}

	void AnimatedSprite::Initialize()
	{
		Sprite::Initialize();
	}

	void AnimatedSprite::Uninitialize()
	{
		Sprite::Uninitialize();
		AnimatedSpriteLibrary::Uninitialize();
	}

	AnimatedSpriteLibrary::LibraryMap AnimatedSprite::s_library;

}
