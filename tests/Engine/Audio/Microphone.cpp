#include <Nazara/Audio/Microphone.hpp>
#include <Catch/catch.hpp>

#define NAZARA_AUDIO_OPENAL
#include <Nazara/Audio/OpenAL.hpp>
#include <Nazara/Core/String.hpp>

#include <Nazara/Audio/Sound.hpp>
#include <Nazara/Core/Thread.hpp>

SCENARIO("Microphone", "[AUDIO][MICROPHONE]")
{
	GIVEN("A default microphone")
	{

		std::vector<Nz::String> inputDevices;
		Nz::OpenAL::QueryInputDevices(inputDevices);
		Nz::Microphone microphone(inputDevices[0]);
		REQUIRE(microphone.GetDeviceName() == inputDevices[0]);

		WHEN("We record 500ms")
		{
			REQUIRE(!microphone.IsRecording());
			microphone.Record();
			REQUIRE(microphone.IsRecording());
			Nz::Thread::Sleep(1000);
			REQUIRE(microphone.IsRecording());
			microphone.Stop();

			THEN("We can play what we registered")
			{
				Nz::ByteArray byteArray;
				microphone.GetSamples(byteArray);
				REQUIRE(microphone.GetFormat() == Nz::RecordingFormat_Stereo16);

				Nz::SoundBuffer recorded(Nz::AudioFormat_Stereo, byteArray.GetSize() / sizeof(Nz::Int16), microphone.GetSampleRate(), (const Nz::Int16*)byteArray.GetConstBuffer());
				Nz::Sound sound(&recorded);
				sound.Play();
			}
		}		
	}
}
