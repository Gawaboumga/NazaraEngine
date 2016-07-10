#include "Map.hpp"

#include "Entity.hpp"
#include "Tile.hpp"

namespace SMB
{

	Map::Map(const Matrix<Nz::String, 2>& map) :
		m_matrix{{ static_cast<dimension_t>(map.ith_dimension(0)), static_cast<Map::dimension_t>(map.ith_dimension(1)) }}
	{
		for (auto i = 0; i < GetWidth(); ++i)
		{
			for (auto j = 0; j < GetHeight(); ++j)
			{
				Tile tile{ map({i, j}) };
				m_matrix({i, j}) = std::move(tile);
			}
		}
	}

	Map::dimension_t Map::GetHeight() const
	{
		return m_matrix.ith_dimension(1);
	}

	Nz::Vector2f Map::GetPossibleMove(const SMB::Entity& entity, float elapsedTime) const
	{
		auto position = entity.GetPosition();
		auto lastGood = Nz::Vector2f{ 0.f, 0.f };
		for (auto i = 0.f; i < elapsedTime; i += 0.001f)
		{
			auto resultingPosition = entity.GetNextPosition(i);
			auto characterDimensions = entity.GetDimensions();
			resultingPosition.x = Nz::Clamp(resultingPosition.x, 0.f, static_cast<float>(GetWidth()) - characterDimensions.x);
			resultingPosition.y = Nz::Clamp(resultingPosition.y, 0.f, static_cast<float>(GetHeight()) - characterDimensions.y);

			Nz::Rectf collisionBox{ resultingPosition.x, resultingPosition.y, characterDimensions.x, characterDimensions.y };
			if (IsColliding(collisionBox))
				break;
			else
				lastGood = resultingPosition - position;
		}

		return lastGood;
	}

	Map::dimension_t Map::GetWidth() const
	{
		return m_matrix.ith_dimension(0);
	}

	bool Map::IsTouchingGround(const SMB::Entity& entity) const
	{
		auto position = entity.GetPosition() + Nz::Vector2f{ 0.f, 0.1f }; // Slightly offset for rounding errors
		auto dimensions = entity.GetDimensions();
		return IsColliding({ position.x, position.y, dimensions.x, dimensions.y });
	}

	Map::id& Map::operator()(dimension_t i, dimension_t j)
	{
		return m_matrix({i, j});
	}

	const Map::id& Map::operator()(dimension_t i, dimension_t j) const
	{
		return m_matrix({i, j});
	}

	const SMB::Tile& Map::GetTile(const Nz::Vector2f& position) const
	{
		Nz::Vector2i integerPosition(position);
		integerPosition.x = Nz::Clamp(integerPosition.x, 0, GetWidth() - 1);
		integerPosition.y = Nz::Clamp(integerPosition.y, 0, GetHeight() - 1);
		return operator()(integerPosition.x, integerPosition.y);
	}

	bool Map::IsColliding(const Nz::Rectf& collisionBox) const
	{
		const auto& tileLB = GetTile(collisionBox.GetCorner(Nz::RectCorner_LeftBottom));
		const auto& tileRB = GetTile(collisionBox.GetCorner(Nz::RectCorner_RightBottom));
		const auto& tileLT = GetTile(collisionBox.GetCorner(Nz::RectCorner_LeftTop));
		const auto& tileRT = GetTile(collisionBox.GetCorner(Nz::RectCorner_RightTop));

		return tileLB.IsBlock() || tileRB.IsBlock() || tileLT.IsBlock() || tileRT.IsBlock();
	}
}
