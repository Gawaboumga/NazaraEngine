#define CATCH_CONFIG_RUNNER
#include <Catch/catch.hpp>

#include <Nazara/Audio/Audio.hpp>
#include <Nazara/Graphics/Graphics.hpp>

int main(int argc, char* const argv[])
{
	Nz::Initializer<Nz::Audio, Nz::Graphics> modules;

	int result = Catch::Session().run(argc, argv);

	return result;
}