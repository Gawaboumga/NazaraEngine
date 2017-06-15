#include <NDK/Components/DebugComponent.hpp>
#include <NDK/World.hpp>
#include <Catch/catch.hpp>

SCENARIO("DebugComponent", "[NDK][DEBUGCOMPONENT]")
{
	GIVEN("A world and an entity with debug component")
	{
		Ndk::World world;
		const Ndk::EntityHandle& entity = world.CreateEntity();
		Ndk::DebugComponent& debugComponent = entity->AddComponent<Ndk::DebugComponent>();

		WHEN("We disable some flags")
		{
			REQUIRE(debugComponent.Has(Ndk::DebugMode_Graphics));
			debugComponent.Disable(Ndk::DebugMode_Graphics);

			THEN("They are no more present")
			{
				REQUIRE(!debugComponent.Has(Ndk::DebugMode_Graphics));
			}

			AND_THEN("Disable it twice, should behave as only once")
			{
				debugComponent.Disable(Ndk::DebugMode_Graphics);
				REQUIRE(!debugComponent.Has(Ndk::DebugMode_Graphics));
			}
		}

		WHEN("We enable some flags")
		{
			REQUIRE(debugComponent.Has(Ndk::DebugMode_Graphics));
			debugComponent.Disable(Ndk::DebugMode_Graphics);
			REQUIRE(!debugComponent.Has(Ndk::DebugMode_Graphics));

			debugComponent.Enable(Ndk::DebugMode_Graphics);
			THEN("They are present")
			{
				REQUIRE(debugComponent.Has(Ndk::DebugMode_Graphics));
			}
		}

		WHEN("We perform an action every second")
		{
			int tmp = 0;
			debugComponent.Every(1000, [&tmp](const Ndk::EntityHandle& /*entity*/, float elapsedTime) {
				tmp += 1;
			});

			THEN("Two actions should be performed in 2 seconds")
			{
				world.Update(1.5f);
				REQUIRE(tmp == 1);
				world.Update(1.0f);
				REQUIRE(tmp == 2);
			}
		}
	}
}