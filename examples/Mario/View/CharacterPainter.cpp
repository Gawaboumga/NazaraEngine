#include "CharacterPainter.hpp"

#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/World.hpp>

#include "../Core/StateContext.hpp"
#include "../Core/Character.hpp"
#include "../Core/URL.hpp"
#include "Dimensions.hpp"
#include "SpriteManager.hpp"

namespace SMB
{
	CharacterPainter::CharacterPainter(SMB::StateContext& context) :
    	m_context{ context }
	{
	}

	void CharacterPainter::Clear()
	{
		for (auto& pair : m_characterEntityMap)
			pair.second->Kill();
		m_characterEntityMap.clear();
	}

	bool CharacterPainter::CreateCharacter(const Character& character)
	{
		Nz::Vector2f marioSize = Nz::Vector2f(Dimensions::WorldScale() * character.GetDimensions());
		auto marioSprite = SpriteManager::Get(SpriteType::Mario);
		marioSprite->SetSize(marioSize);

		auto entity = m_context.world.CreateEntity();
		auto& nodeComponent = entity->AddComponent<Ndk::NodeComponent>();
		nodeComponent.SetPosition(character.GetPosition() * Dimensions::WorldScale());
		auto& graphicsComponent = entity->AddComponent<Ndk::GraphicsComponent>();
		graphicsComponent.Attach(marioSprite);

		m_characterEntityMap.insert(std::make_pair(character.GetID(), entity));

		return true;
	}

	void CharacterPainter::Update(const Character& character, float elapsedTime)
	{
		auto& entity = GetEntityAssociatedWith(character);
		if (!character.IsAlive())
		{
			if (entity->IsEnabled())
				entity->Enable(false);

			return;
		}
		auto& nodeComponent = entity->GetComponent<Ndk::NodeComponent>();
		nodeComponent.SetPosition(character.GetPosition() * Dimensions::WorldScale());
	}

	Ndk::EntityHandle& CharacterPainter::GetEntityAssociatedWith(const Character& character)
	{
		return m_characterEntityMap[character.GetID()];
	}
}
