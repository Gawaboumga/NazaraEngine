#include <Nazara/Graphics/TileMap.hpp>
#include <NDK/Application.hpp>
#include <NDK/Components.hpp>
#include <NDK/Systems.hpp>
#include <NDK/World.hpp>
#include <Nazara/Renderer/RenderWindow.hpp>
#include <Catch/catch.hpp>

void AddBindingCamera(Nz::RenderWindow& window, const Ndk::EntityHandle& camera, const Nz::TileMapRef tileMap);
Ndk::EntityHandle AddCamera(Ndk::World& world, const Nz::RenderWindow& window);

SCENARIO("TileMap", "[GRAPHICS][TILEMAP]")
{
	GIVEN("A tile map with the layer and two materials")
	{
		Nz::Vector2ui mapSize(10, 20);
		Nz::Vector2f tileSize(5.f, 15.f);
		std::size_t layerCount = 3;
		std::size_t materialCount = 2;
		Nz::TileMap tileMap(mapSize, tileSize, layerCount, materialCount);

		WHEN("We ask for common information")
		{
			THEN("Those results are expected")
			{
				CHECK(tileMap.GetMaterialCount() == materialCount);
				CHECK(tileMap.GetMapSize() == mapSize);
				CHECK(tileMap.GetTileSize() == tileSize);
				CHECK(tileMap.IsIsometricModeEnabled() == false);
			}
		}

		WHEN("We enable all the tiles")
		{
			tileMap.EnableTiles(0, Nz::Rectf::Zero(), Nz::Color::Red);
			tileMap.EnableTiles(1, Nz::Rectf::Zero(), Nz::Color::Blue);
			THEN("They should be correct")
			{
				const Nz::TileMap::Tile& tile = tileMap.GetTile(0, Nz::Vector2ui::Zero());
				REQUIRE(tile.index == 0);
				REQUIRE(tile.color == Nz::Color::Red);

				const Nz::TileMap::Tile& tile2 = tileMap.GetTile(1, Nz::Vector2ui::Zero());
				REQUIRE(tile2.index == 1 * materialCount);
				REQUIRE(tile2.color == Nz::Color::Blue);
			}
		}

		WHEN("We enable a specific tile")
		{
			Nz::Vector2ui position(0, 1);
			tileMap.EnableTile(0, position, Nz::Rectf::Zero(), Nz::Color::Red);
			tileMap.EnableTile(1, position, Nz::Rectf::Zero(), Nz::Color::Blue);
			THEN("They should be correct")
			{
				const Nz::TileMap::Tile& tile = tileMap.GetTile(0, position);
				REQUIRE(tile.index == 0);
				REQUIRE(tile.color == Nz::Color::Red);

				const Nz::TileMap::Tile& tile2 = tileMap.GetTile(1, position);
				REQUIRE(tile2.index == 1 * materialCount);
				REQUIRE(tile2.color == Nz::Color::Blue);
			}
		}

		WHEN("We set a material")
		{
			std::size_t layer = 1;
			std::size_t materialIndex = 1;
			Nz::MaterialRef material = Nz::Material::New("Translucent2D");
			tileMap.SetMaterial(layer, materialIndex, material);

			THEN("We should retrieve it")
			{
				REQUIRE(tileMap.GetMaterial(layer, 0) != material);
				REQUIRE(tileMap.GetMaterial(layer, materialIndex) == material);
			}
		}
	}

	GIVEN("Bar")
	{

		Ndk::Application& app = *Ndk::Application::Instance();
		auto& window = app.AddWindow<Nz::RenderWindow>();
		if (!window.Create(Nz::VideoMode(1024, 768, 32), "TileMap"))
		{
			NazaraError("Failed to create window. See NazaraLog.log for further informations");
			REQUIRE(false);
		}
		window.EnableVerticalSync(true);

		auto& world = app.AddWorld();
		world.GetSystem<Ndk::RenderSystem>().SetGlobalUp(Nz::Vector3f::Down());

		Ndk::EntityHandle camera = AddCamera(world, window);

		Ndk::EntityHandle map = world.CreateEntity();
		map->AddComponent<Ndk::NodeComponent>();
		Ndk::GraphicsComponent& graphicsComponent = map->AddComponent<Ndk::GraphicsComponent>();

		Nz::TileMapRef tileMap = Nz::TileMap::New();
		//REQUIRE(tileMap->LoadFromFile("resources/examples/desert.tmx"));
		//REQUIRE(tileMap->LoadFromFile("resources/examples/orthogonal-outside.tmx"));
		REQUIRE(tileMap->LoadFromFile("resources/examples/sewers.tmx"));
		graphicsComponent.Attach(tileMap);
		/*Nz::MaterialRef re = Nz::Material::New();
		re->LoadFromFile("resources/examples/buch-outdoor.png");
		//re->Configure("Translucent2D");
		re->EnableBlending(true);
		re->SetSrcBlend(Nz::BlendFunc_SrcAlpha);
		Nz::SpriteRef sprite = Nz::Sprite::New(re);
		graphicsComponent.Attach(sprite);*/

		AddBindingCamera(window, camera, tileMap);

		while (app.Run())
		{
			window.Display();
		}
	}
}

Ndk::EntityHandle AddCamera(Ndk::World& world, const Nz::RenderWindow& window)
{
	// We create our 2D camera which will render in our window
	Ndk::EntityHandle camera(world.CreateEntity());
	// We need to add a position to the entity
	camera->AddComponent<Ndk::NodeComponent>();
	// And, of course, say that it is a special entity to film
	Ndk::CameraComponent& CameraComponent = camera->AddComponent<Ndk::CameraComponent>();
	CameraComponent.SetProjectionType(Nz::ProjectionType_Orthogonal); // 2D view (by default 3D)
	CameraComponent.SetTarget(&window); // We say to broadcast what we film to that window
	return camera;
}

void AddBindingCamera(Nz::RenderWindow& window, const Ndk::EntityHandle& camera, const Nz::TileMapRef tileMap)
{
	Nz::EventHandler& eventHandler = window.GetEventHandler();
	eventHandler.OnKeyPressed.Connect([&](const Nz::EventHandler*, const Nz::WindowEvent::KeyEvent& key)
	{
		Nz::Vector3f position = camera->GetComponent<Ndk::NodeComponent>().GetPosition();
		float scale = 10.f;

		if (key.code == Nz::Keyboard::Q)
			position -= Nz::Vector3f::UnitX() * scale;
		if (key.code == Nz::Keyboard::D)
			position += Nz::Vector3f::UnitX() * scale;
		if (key.code == Nz::Keyboard::Z)
			position -= Nz::Vector3f::UnitY() * scale;
		if (key.code == Nz::Keyboard::S)
			position += Nz::Vector3f::UnitY() * scale;
		
		position.x = Nz::Clamp(position.x, 0.f, tileMap->GetSize().x - window.GetSize().x);
		position.y = Nz::Clamp(position.y, 0.f, tileMap->GetSize().y - window.GetSize().y);
		camera->GetComponent<Ndk::NodeComponent>().SetPosition(position);
	});
}