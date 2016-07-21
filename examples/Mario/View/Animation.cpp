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

#include "Animation.hpp"

namespace SMB
{

	void Animation::AddFrame(const Nz::Rectui& area)
	{
		m_frames.push_back(area);
	}

	const Nz::Rectui& Animation::GetFrame(int i) const
	{
		#ifdef NAZARA_GRAPHICS_SAFE
		if (i >= m_frames.size())
		{
			NazaraError("Index out of range: (" + String::Number(i) + ") > (" + String::Number(m_frames.size()) + ')');
			return {};
		}
		#endif // NAZARA_GRAPHICS_SAFE

		return m_frames[i];
	}

	int Animation::GetSize() const
	{
		return m_frames.size();
	}

	const Nz::TextureRef Animation::GetSpriteSheet() const
	{
		return m_spriteSheetTexture;
	}

	void Animation::SetSpriteSheet(const Nz::TextureRef texture)
	{
		m_spriteSheetTexture = texture;
	}

}
