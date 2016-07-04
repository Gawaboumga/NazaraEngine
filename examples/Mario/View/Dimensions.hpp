#ifndef __DIMENSIONS_HPP__
#define __DIMENSIONS_HPP__

namespace SMB
{
	struct Dimensions
	{
		static Nz::Vector2f GetTiles()
		{
			return { 32.f, 32.f };
		}

		static Nz::Vector2f GetCharacter()
		{
			return { 16.f, 24.f };
		}
	};
}

#endif // __DIMENSIONS_HPP__
