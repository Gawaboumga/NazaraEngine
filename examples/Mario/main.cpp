#include <Nazara/Renderer/RenderWindow.hpp>

#include <NDK/Application.hpp>
#include <NDK/StateMachine.hpp>
#include <NDK/World.hpp>
#include <NDK/Components/CameraComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Systems/RenderSystem.hpp>

#include "States/StateFactory.hpp"
#include "Core/Player.hpp"
#include "Core/StateContext.hpp"

#include <iostream>

Ndk::EntityHandle AddCamera(Ndk::World& world, Nz::RenderWindow& window);

int main()
{
	Ndk::Application app{};
	auto& window = app.AddWindow<Nz::RenderWindow>();
	if (!window.Create(Nz::VideoMode{800, 600, 32}, "Mario"))
	{
		std::cerr << "Failed to create window. See NazaraLog.log for further informations" << std::endl;
		return EXIT_FAILURE;
	}
	window.EnableVerticalSync(true);

	auto& world = app.AddWorld();

	auto camera = AddCamera(world, window);

	SMB::KeyBinding keyBinding;
	SMB::Player mario{ &keyBinding };
	SMB::StateContext context{ window, world, camera, mario };
	SMB::StateFactory::Initialize(context);
    Ndk::StateMachine fsm{ SMB::StateFactory::Get(SMB::State::Game) };

	Nz::Clock elapsedTimeClock, fpsClock;
	Nz::String windowTitle{ "Mario" };
	int fps = 0;
    while (app.Run())
	{
		float elapsedTime{ elapsedTimeClock.GetSeconds() };
		elapsedTimeClock.Restart();

        if (!fsm.Update(elapsedTime))
		{
            std::cerr << "Failed to update state machine." << std::endl;
            return EXIT_FAILURE;
        }

        if (Nz::Keyboard::IsKeyPressed(Nz::Keyboard::Escape))
			app.Quit();

        Nz::WindowEvent event;
		while (window.PollEvent(&event))
		{
			if (event.type == Nz::WindowEventType_Quit)
				app.Quit();
		}

        window.Display();

		fps++;

		if (fpsClock.GetMilliseconds() >= 1000)
		{
			window.SetTitle(windowTitle + " - " + Nz::String::Number(fps) + " FPS");
			fps = 0;
			fpsClock.Restart();
		}
    }

	return EXIT_SUCCESS;
}

Ndk::EntityHandle AddCamera(Ndk::World& world, Nz::RenderWindow& window)
{
	Ndk::EntityHandle view{ world.CreateEntity() };
	view->AddComponent<Ndk::NodeComponent>();
	auto& cam = view->AddComponent<Ndk::CameraComponent>();
	cam.SetProjectionType(Nz::ProjectionType_Orthogonal); // 2D
	cam.SetTarget(&window);

	return view;
}
