#include "Level.hpp"

#include "CollisionHelper.hpp"
#include "LevelData.hpp"
#include "Player.hpp"
#include "StateContext.hpp"
#include "TMXParser.hpp"
#include "URL.hpp"

namespace SMB
{

	Level::Level(SMB::StateContext& context) :
		m_context{ context }
	{
	}

	void Level::AddPlayer()
	{
		auto character = Character::MakeSpawn({ 5, 5 });
		m_characters.push_back(std::move(character));
	}

	CommandQueue& Level::GetCommandQueue()
	{
		return m_commandQueue;
	}

	const std::vector<Character>& Level::GetCharacters() const
	{
		return m_characters;
	}

	const std::vector<Coin>& Level::GetCoins() const
	{
		return m_coins;
	}

	const std::vector<Enemy>& Level::GetEnemies() const
	{
		return m_enemies;
	}

	Level::Info Level::GetInfo() const
	{
		return m_info;
	}

	const SMB::Map& Level::GetMap() const
	{
		return m_map;
	}

	bool Level::HasAlivePlayer() const
	{
		return std::any_of(m_characters.begin(), m_characters.end(), [](const Character& character) {
			return character.IsAlive();
		});
	}

	bool Level::HasPlayerReachedEnd() const
	{
		return false;
	}

	bool Level::Load(Info level)
	{
		Unload();

		auto fileLevel = URL::GetTMXPath(level);
		auto map = TMXParser::GetMap(fileLevel);
		m_map = Map{ map };
		m_info = level;

		auto fileEnemyLevel = URL::GetEnemyPath(level);
		auto enemies = TMXParser::GetEnemies(fileEnemyLevel);
		m_enemies = std::move(enemies);

		auto fileCoinLevel = URL::GetCoinPath(level);
		auto coins = TMXParser::GetCoins(fileCoinLevel);
		m_coins = std::move(coins);

		return true;
	}

	Nz::Vector2f Level::Move(SMB::Entity& entity, float elapsedTime)
	{
		auto resultingDirection = m_map.GetPossibleMove(entity, elapsedTime);
		entity.Move(resultingDirection);
		if (entity.GetPosition().y >= m_map.GetHeight() - 1)
			entity.TakeDamage();
		return resultingDirection;
	}

	void Level::Update(float elapsedTime)
	{
		for (auto& coin : m_coins)
			coin.Update(elapsedTime);

		for (auto& enemy : m_enemies)
		{
			enemy.Update(elapsedTime);
			Move(enemy, elapsedTime);
		}

		for (auto& character : m_characters)
		{
			character.Update(elapsedTime);
			Move(character, elapsedTime);
		}

		while (!m_commandQueue.IsEmpty())
		{
			auto command = m_commandQueue.Pop();
			for (auto& character : m_characters)
				command(*this, character, elapsedTime);
		}

		ApplyGravity(elapsedTime);

		HandleEntitiesCollision(elapsedTime);
	}

	void Level::ApplyGravity(float elapsedTime)
	{
		const Nz::Vector2f gravity{ 0.f, 9.81f };
		for (auto& enemy : m_enemies)
		{
			enemy.Accelerate(gravity, elapsedTime);
			Move(enemy, elapsedTime);
			if (m_map.IsTouchingGround(enemy))
				enemy.TouchGround(true);
			else
				enemy.TouchGround(false);
		}

		for (auto& character : m_characters)
		{
			character.Accelerate(gravity, elapsedTime);
			Move(character, elapsedTime);
			if (m_map.IsTouchingGround(character))
				character.TouchGround(true);
			else
				character.TouchGround(false);
		}
	}

	void Level::HandleEntitiesCollision(float elapsedTime)
	{
		for (auto& character : m_characters)
		{
			if (!character.IsAlive())
				continue;

			for (auto& enemy : m_enemies)
			{
				if (!enemy.IsAlive())
					continue;

				Collision collision = Collide(character, enemy);
				if (collision == Collision::None)
					continue;

				if (collision == Collision::Up)
				{
					character.ResetVelocity();
					character.Accelerate({ 0.f, -50.f }, elapsedTime);
					enemy.TakeDamage();
					m_context.player.AddScore(enemy.GetScore());
				}
				else
					character.TakeDamage();
			}

			for (auto& coin : m_coins)
			{
				if (!coin.IsAlive())
					continue;

				Collision collision = Collide(character, coin);
				if (collision == Collision::None)
					continue;

				coin.TakeDamage();
				m_context.player.AddCoin();
			}
		}
	}

	void Level::Unload()
	{
		m_characters.clear();
		m_coins.clear();
		m_enemies.clear();
	}
}
