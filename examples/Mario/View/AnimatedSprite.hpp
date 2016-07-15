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

#ifndef __ANIMATEDSPRITE_HPP__
#define __ANIMATEDSPRITE_HPP__

#include <Nazara/Graphics/Sprite.hpp>

#include "Animation.hpp"

namespace SMB
{
	class AnimatedSprite : public Nz::Sprite
	{
		public:

			explicit AnimatedSprite(float frameTime = 0.2f, bool paused = false, bool looped = true);

			const Animation* GetAnimation() const;
			float GetFrameTime() const;

			bool IsLooped() const;
			bool IsPlaying() const;

			void Update(float deltaTime);
			void SetAnimation(const Animation& animation);
			void SetFrameTime(float time);
			void Play();
			void Play(const Animation& animation);
			void Pause();
			void Stop();
			void SetLooped(bool looped);

			void SetFrame(std::size_t newFrame, bool resetTime = true);

			template <typename... Args>
			static Nz::ObjectRef<AnimatedSprite> New(Args&&... args)
			{
				std::unique_ptr<AnimatedSprite> object(new AnimatedSprite(std::forward<Args>(args)...));
				object->SetPersistent(false);

				return object.release();
			}

		private:
			const Animation* m_animation;
			float m_frameTime;
			float m_currentTime;
			int m_currentFrame;
			bool m_isPaused;
			bool m_isLooped;
	};
}

#endif // __ANIMATEDSPRITE_HPP__
