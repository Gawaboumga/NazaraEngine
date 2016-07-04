#include "Level.hpp"

#include "LevelData.hpp"
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
		return true;
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

		return true;
	}

	Nz::Vector2f Level::Move(SMB::Character& character, float elapsedTime)
	{
		auto resultingDirection = m_map.GetPossibleMove(character, elapsedTime);
		character.Move(resultingDirection);
		return resultingDirection;
	}

	void Level::Update(float elapsedTime)
	{
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
	}

	void Level::ApplyGravity(float elapsedTime)
	{
		const Nz::Vector2f gravity{ 0.f, 9.81f };
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

	void Level::Unload()
	{
	}
}
