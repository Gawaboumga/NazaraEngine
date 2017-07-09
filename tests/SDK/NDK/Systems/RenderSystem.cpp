#include <NDK/Systems/RenderSystem.hpp>
#include <NDK/World.hpp>
#include <NDK/Components.hpp>
#include <Nazara/Graphics/Sprite.hpp>
#include <Catch/catch.hpp>

SCENARIO("RenderSystem", "[NDK][RenderSystem]")
{
	GIVEN("A world with a camera, a drawable, a light and some particles")
	{
		Ndk::World world;
		const Ndk::EntityHandle& cameraEntity = world.CreateEntity();
		Ndk::CameraComponent& cameraComponentCamera = cameraEntity->AddComponent<Ndk::CameraComponent>();
		Ndk::NodeComponent& nodeComponentCamera = cameraEntity->AddComponent<Ndk::NodeComponent>();

		const Ndk::EntityHandle& drawableEntity = world.CreateEntity();
		Ndk::GraphicsComponent& graphicsComponentDrawable = drawableEntity->AddComponent<Ndk::GraphicsComponent>();
		Nz::SpriteRef sprite = Nz::Sprite::New();
		graphicsComponentDrawable.Attach(sprite);
		Ndk::NodeComponent& nodeComponentDrawable = drawableEntity->AddComponent<Ndk::NodeComponent>();

		const Ndk::EntityHandle& lightEntity = world.CreateEntity();
		Ndk::LightComponent& lightComponentLight = lightEntity->AddComponent<Ndk::LightComponent>();
		Ndk::NodeComponent& nodeComponentLight = lightEntity->AddComponent<Ndk::NodeComponent>();

		const Ndk::EntityHandle& particlesEntity = world.CreateEntity();
		Ndk::ParticleGroupComponent& particleGroupComponentParticles = particlesEntity->AddComponent<Ndk::ParticleGroupComponent>(1, Nz::ParticleLayout_Sprite);

		WHEN("We change the render technique to ForwardRenderTechnique")
		{
			Ndk::RenderSystem& renderSystem = world.GetSystem<Ndk::RenderSystem>();
			renderSystem.ChangeRenderTechnique<Nz::ForwardRenderTechnique>();

			THEN("The render system should be ForwardRenderTechnique")
			{
				REQUIRE(renderSystem.GetRenderTechnique().GetType() == Nz::RenderTechniqueType_BasicForward);
			}
		}
	}

	GIVEN("A world and an entity with graphics and physics")
	{
		Ndk::World world;
		const Ndk::EntityHandle& entity = world.CreateEntity();

		Nz::Vector2f position(200.f, 300.f);
		Ndk::NodeComponent& nodeComponent = entity->AddComponent<Ndk::NodeComponent>();
		nodeComponent.SetPosition(position);

		Nz::Vector2f dimensions(10.f, 20.f);
		Ndk::GraphicsComponent& graphicsComponent = entity->AddComponent<Ndk::GraphicsComponent>();
		Nz::SpriteRef sprite = Nz::Sprite::New();
		sprite->SetSize(dimensions);
		graphicsComponent.Attach(sprite);

		Nz::Rectf aabb(Nz::Vector2f::Zero(), dimensions);
		Nz::BoxCollider2DRef boxCollider2D = Nz::BoxCollider2D::New(aabb);
		entity->AddComponent<Ndk::CollisionComponent2D>(boxCollider2D);
		Ndk::PhysicsComponent2D& physicsComponent2D = entity->AddComponent<Ndk::PhysicsComponent2D>();

		world.Update(1.f);

		WHEN("We make rotate it")
		{
			physicsComponent2D.SetAngularVelocity(Nz::FromDegrees(45.f));
			world.Update(2.f);

			THEN("Graphics and physics should be synchronised")
			{
				Nz::Boxf physicsCollisionBox(physicsComponent2D.GetAABB());
				physicsCollisionBox.y = physicsCollisionBox.y + physicsCollisionBox.height;
				Nz::Boxf graphicsCollisionBox = graphicsComponent.GetBoundingVolume().aabb;
				CHECK(physicsCollisionBox.x == Approx(graphicsCollisionBox.x));
				CHECK(physicsCollisionBox.y == Approx(graphicsCollisionBox.y));
				CHECK(physicsCollisionBox.width == Approx(graphicsCollisionBox.width));
				CHECK(physicsCollisionBox.height == Approx(graphicsCollisionBox.height));
			}
		}
	}
}