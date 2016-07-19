#ifndef __COLLISIONHELPER_HPP__
#define __COLLISIONHELPER_HPP__

#include "Entity.hpp"

namespace SMB
{
	enum class Collision
	{
		None,
		Down,
		Left,
		Right,
		Up
	};

	Nz::String ToString(Collision collision);
	Collision Collide(const SMB::Entity& lhs, const SMB::Entity& rhs);

	Nz::String ToString(Collision collision)
	{
		switch (collision)
		{
			case Collision::None:
				return "None";
			case Collision::Down:
				return "Down";
			case Collision::Left:
				return "Left";
			case Collision::Right:
				return "Right";
			case Collision::Up:
				return "Up";
		}
		return "ERROR";
	}

	Collision Collide(const SMB::Entity& lhs, const SMB::Entity& rhs)
	{
		auto lhsPosition = lhs.GetPosition();
		auto lhsDimension = lhs.GetDimensions();
		Nz::Rectf lhsAABB{ lhsPosition.x, lhsPosition.y, lhsDimension.x, lhsDimension.y };

		auto rhsPosition = rhs.GetPosition();
		auto rhsDimension = rhs.GetDimensions();
		Nz::Rectf rhsAABB{ rhsPosition.x, rhsPosition.y, rhsDimension.x, rhsDimension.y };

		if (!lhsAABB.Intersect(rhsAABB))
			return Collision::None;
		else
		{
			Nz::Vector2f upperMiddlePoint{
				rhsPosition.x + rhsDimension.x / 2.f,
				rhsPosition.y
			};
			if (lhsPosition.y < upperMiddlePoint.y)
				return Collision::Up;
			else if (lhsPosition.x < upperMiddlePoint.x)
				return Collision::Left;
			else if (lhsPosition.x > upperMiddlePoint.x)
				return Collision::Right;
			else
				return Collision::Down;
		}
	}
}

#endif // __COLLISIONHELPER_HPP__
