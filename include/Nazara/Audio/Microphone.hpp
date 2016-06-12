// Copyright (C) 2015 Jérôme Leclercq
// This file is part of the "Nazara Engine - Audio module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_MICROPHONE_HPP
#define NAZARA_MICROPHONE_HPP

#include <Nazara/Prerequesites.hpp>
#include <Nazara/Audio/Enums.hpp>
#include <Nazara/Audio/Config.hpp>

namespace Nz
{
	class ByteArray;
	class String;
	class MicrophoneImpl;

	class NAZARA_AUDIO_API Microphone
	{
		public:
			Microphone(RecordingFormat recordingFormat = RecordingFormat_Stereo16, UInt32 sampleRate = 44100);
			Microphone(const Nz::String& deviceName, RecordingFormat recordingFormat = RecordingFormat_Stereo16, UInt32 sampleRate = 44100);
			Microphone(const Microphone&) = delete;
			Microphone(Microphone&&) = delete;
			~Microphone();

			String GetDeviceName() const;
			RecordingFormat GetFormat() const;
			UInt32 GetSampleRate() const;
			void GetSamples(Nz::ByteArray& output);

			bool IsRecording() const;

			void Record();

			void Stop();

			Microphone& operator=(const Microphone&) = delete;
			Microphone& operator=(Microphone&&) = delete;

		private:
			UInt32 GetFormatSize(RecordingFormat recordingFormat);

			void MicrophoneThread();

			MicrophoneImpl* m_impl = nullptr;
	};
}

#endif // NAZARA_MICROPHONE_HPP
