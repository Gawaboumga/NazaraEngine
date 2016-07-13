#ifndef __DIMENSIONS_HPP__
#define __DIMENSIONS_HPP__

namespace SMB
{
	struct Dimensions
	{
		static Nz::Vector2f WorldScale()
		{
			return { 32.f, 32.f };
		}
	};
}

#endif // __DIMENSIONS_HPP__
