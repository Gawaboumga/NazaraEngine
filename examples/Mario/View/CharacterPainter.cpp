#include "CharacterPainter.hpp"

#include <Nazara/Graphics/Sprite.hpp>

#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/World.hpp>

#include "../Core/StateContext.hpp"
#include "../Core/Character.hpp"
#include "Dimensions.hpp"

namespace SMB
{
	CharacterPainter::CharacterPainter(SMB::StateContext& context) :
    	m_context{ context }
	{
	}

	bool CharacterPainter::CreateCharacter(const Character& character)
	{
		Nz::String mario{ "Mario" };
		Nz::Vector2ui marioSize = Nz::Vector2ui(Dimensions::GetTiles() * character.GetDimensions());

		if (!Nz::ImageLibrary::Has(mario))
		{
			auto image = Nz::Image::New();
			if (!image->LoadFromFile("/home/yhubaut/NazaraEngine/examples/bin/resources/Mario/Elements/smb_mario_sheet.png"))
			{
				NazaraError("Error while loading assets");
				return false;
			}

			Nz::ImageLibrary::Register(mario, image);
		}

		if (!Nz::SpriteLibrary::Has(mario))
		{
			auto image = Nz::ImageLibrary::Get(mario);
			auto marioImage = Nz::Image::New(image->GetType(), image->GetFormat(), marioSize.x, marioSize.y);
			Nz::Boxui box{
				208, 0, 0, // Hard coded position of Mario
				marioSize.x, marioSize.y, 1
			};
			marioImage->Copy(*image.Get(), box, Nz::Vector3ui::Zero());

			auto marioTexture = Nz::Texture::New(*marioImage.Get());
			Nz::TextureLibrary::Register(mario, marioTexture);
			auto marioSprite = Nz::Sprite::New(marioTexture.Get());
			Nz::SpriteLibrary::Register(mario, marioSprite);
		}

		auto marioSprite = Nz::SpriteLibrary::Get(mario);

		auto entity = m_context.world.CreateEntity();
		auto& nodeComponent = entity->AddComponent<Ndk::NodeComponent>();
		nodeComponent.SetPosition(character.GetPosition() * Dimensions::GetTiles());
		auto& graphicsComponent = entity->AddComponent<Ndk::GraphicsComponent>();
		graphicsComponent.Attach(marioSprite);

		m_characterEntityMap[character.GetID()] = entity;

		return true;
	}

	void CharacterPainter::Update(const Character& character, float elapsedTime)
	{
		auto& entity = GetEntityAssociatedWith(character);
		auto& nodeComponent = entity->GetComponent<Ndk::NodeComponent>();
		nodeComponent.SetPosition(character.GetPosition() * Dimensions::GetTiles());
	}

	Ndk::EntityHandle& CharacterPainter::GetEntityAssociatedWith(const Character& character)
	{
		return m_characterEntityMap[character.GetID()];
	}
}
