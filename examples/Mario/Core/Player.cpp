#include "Player.hpp"

namespace SMB
{

	struct Mover
	{
		Mover(const Nz::Vector2f& acceleration) :
			m_acceleration{ acceleration }
		{
		}

		void operator()(SMB::Level& level, SMB::Character& character, float elapsedTime)
		{
			if (!character.IsTouchingGround())
				m_acceleration.y = 0.f;

			character.Accelerate(m_acceleration * 9.81f, elapsedTime);
			level.Move(character, elapsedTime);
		}

		Nz::Vector2f m_acceleration;
	};

	Player::Player(const KeyBinding* binding) :
		m_level{ 1 },
		m_coins{ 0 },
		m_numberOfLives{ 3 },
		m_score{ 0 },
		m_hasChanged{ true },
		m_keyBinding{ binding }
	{
		InitializeActions();
	}

	void Player::AddCoin()
	{
		m_hasChanged = true;
		m_coins += 1;
	}

	void Player::AddDeath()
	{
		m_hasChanged = true;
	}

	void Player::AddScore(int score)
	{
		m_hasChanged = true;
		m_score += score;
	}

	void Player::FinishLevel()
	{
		m_hasChanged = true;
	}

	int Player::GetCoins() const
	{
		return m_coins;
	}

	Level::Info Player::GetLevel() const
	{
		return m_level;
	}

	int Player::GetNumberOfLives() const
	{
		return m_numberOfLives;
	}

	int Player::GetScore() const
	{
		return m_score;
	}

	void Player::HandleInput(CommandQueue& commandQueue)
	{
		std::vector<Action> activeActions = m_keyBinding->GetRealtimeActions();
		for (auto& action : activeActions)
			commandQueue.Push(m_actionBinding[action]);
	}

	bool Player::HasChanged()
	{
		bool returnChanged = m_hasChanged;
		m_hasChanged = false;
		return returnChanged;
	}

	void Player::InitializeActions()
	{
		m_actionBinding[PlayerAction::MoveLeft].action	= Mover({-1, 0});
		m_actionBinding[PlayerAction::MoveRight].action	= Mover({1, 0});
		m_actionBinding[PlayerAction::MoveDown].action	= Mover({0, 1}); // Inverted Y axis ;-)
		m_actionBinding[PlayerAction::Jump].action		= Mover({0, -50}); // Inverted Y axis ;-)
	}

}
