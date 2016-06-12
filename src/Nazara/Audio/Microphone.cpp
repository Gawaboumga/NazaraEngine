// Copyright (C) 2015 Jérôme Leclercq
// This file is part of the "Nazara Engine - Audio module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Audio/Microphone.hpp>
#include <Nazara/Audio/Config.hpp>
#include <Nazara/Audio/OpenAL.hpp>
#include <Nazara/Core/Error.hpp>
#include <Nazara/Core/Log.hpp>
#include <Nazara/Core/Thread.hpp>
#include <Nazara/Audio/Debug.hpp>

namespace Nz
{
	struct MicrophoneImpl
	{
		ALCdevice* captureDevice = nullptr;
		Nz::ByteArray chunkSamples;
		Mutex bufferLock;
		Thread thread;
		RecordingFormat format;
		UInt32 sampleRate;
		UInt64 processedSamples = 0;
		bool isRecording = false;
	};

	namespace
	{
		/*!
		* \brief Converts the enumeration from OpenAL to Nazara
		* \return OpenAL enumeration corresponding
		*
		* \param recordingFormat Enumeration to convert
		*/

		ALenum ConvertEnumToOpenAL(RecordingFormat recordingFormat)
		{
			switch (recordingFormat)
			{
				case RecordingFormat_Mono8:
					return AL_FORMAT_MONO8;
				case RecordingFormat_Mono16:
					return AL_FORMAT_MONO16;
				case RecordingFormat_Stereo8:
					return AL_FORMAT_STEREO8;
				case RecordingFormat_Stereo16:
					return AL_FORMAT_STEREO16;
				default:
					NazaraInternalError("Invalid recording format (0x" + String::Number(recordingFormat, 16) + ')');
					return AL_FORMAT_MONO8;
			}
		}

		/*!
		* \brief Checks whether the format is in stereo
		* \return true If it is the case
		*/

		bool IsStereo(RecordingFormat recordingFormat)
		{
			switch (recordingFormat)
			{
				case RecordingFormat_Mono8:
				case RecordingFormat_Mono16:
					return false;
				case RecordingFormat_Stereo8:
				case RecordingFormat_Stereo16:
					return true;
				default:
					NazaraInternalError("Invalid recording format (0x" + String::Number(recordingFormat, 16) + ')');
					return false;
			}
		}
	}

	/*!
	* \brief Constructs a Microphone object with a recording format and a sample rate
	*
	* \param recordingFormat Quality of the format
	* \param sampleRate Number of samples per second
	*/

	Microphone::Microphone(RecordingFormat recordingFormat, UInt32 sampleRate)
	{
		m_impl = new MicrophoneImpl;
		m_impl->format = recordingFormat;
		m_impl->sampleRate = sampleRate;
		m_impl->captureDevice = alcCaptureOpenDevice(nullptr, m_impl->sampleRate, ConvertEnumToOpenAL(m_impl->format), m_impl->sampleRate * GetFormatSize(m_impl->format));

		if (!m_impl->captureDevice)
		{
			delete m_impl;
			m_impl = nullptr;
		}
	}

	/*!
	* \brief Constructs a Microphone object with a named device, a recording format and a sample rate
	*
	* \param deviceName Name of the device
	* \param recordingFormat Quality of the format
	* \param sampleRate Number of samples per second
	*/

	Microphone::Microphone(const Nz::String& deviceName, RecordingFormat recordingFormat, UInt32 sampleRate)
	{
		m_impl = new MicrophoneImpl;
		m_impl->format = recordingFormat;
		m_impl->sampleRate = sampleRate;
		m_impl->captureDevice = alcCaptureOpenDevice(deviceName.GetConstBuffer(), m_impl->sampleRate, ConvertEnumToOpenAL(m_impl->format), m_impl->sampleRate * GetFormatSize(m_impl->format));

		if (!m_impl->captureDevice)
		{
			delete m_impl;
			m_impl = nullptr;
		}
	}

	/*!
	* \brief Destructs the object and calls Stop
	*
	* \see Stop
	*/

	Microphone::~Microphone()
	{
		if (m_impl)
		{
			Stop();

			alcCaptureCloseDevice(m_impl->captureDevice);

			delete m_impl;
			m_impl = nullptr;
		}
	}

	/*!
	* \brief Gets the name of the input device
	* \return Input device name
	*
	* \remark Produces a NazaraError with NAZARA_AUDIO_SAFE defined if microphone creation failed
	*/

	Nz::String Microphone::GetDeviceName() const
	{
		#if NAZARA_AUDIO_SAFE
		if (!m_impl)
		{
			NazaraError("Microphone could not be created");
			return "ERROR";
		}
		#endif

		return alcGetString(m_impl->captureDevice, ALC_CAPTURE_DEVICE_SPECIFIER);
	}

	/*!
	* \brief Gets the recording format of the input device
	* \return Recording format
	*
	* \remark Produces a NazaraError with NAZARA_AUDIO_SAFE defined if microphone creation failed
	*/

	RecordingFormat Microphone::GetFormat() const
	{
		#if NAZARA_AUDIO_SAFE
		if (!m_impl)
		{
			NazaraError("Microphone could not be created");
			return RecordingFormat_Unknown;
		}
		#endif

		return m_impl->format;
	}

	/*!
	* \brief Gets the number of samples per second
	* \return Number of samples
	*
	* \remark Produces a NazaraError with NAZARA_AUDIO_SAFE defined if microphone creation failed
	*/

	UInt32 Microphone::GetSampleRate() const
	{
		#if NAZARA_AUDIO_SAFE
		if (!m_impl)
		{
			NazaraError("Microphone could not be created");
			return 0;
		}
		#endif

		return m_impl->sampleRate;
	}

	/*!
	* \brief Gets the samples recorded of the input device
	*
	* \param output Container for the data retrieval
	*
	* \remark Data are lost if they are not polled after one second or more
	* \remark Produces a NazaraError with NAZARA_AUDIO_SAFE defined if microphone creation failed
	*/

	void Microphone::GetSamples(Nz::ByteArray& output)
	{
		#if NAZARA_AUDIO_SAFE
		if (!m_impl)
		{
			NazaraError("Microphone could not be created");
			return;
		}
		#endif

		Nz::LockGuard lock(m_impl->bufferLock);
		output += m_impl->chunkSamples;
	}

	/*!
	* \brief Checks whether the microphone is recording
	* \return true If it is the case
	*
	* \remark Produces a NazaraError with NAZARA_AUDIO_SAFE defined if microphone creation failed
	*/

	bool Microphone::IsRecording() const
	{
		#if NAZARA_AUDIO_SAFE
		if (!m_impl)
		{
			NazaraError("Microphone could not be created");
			return false;
		}
		#endif

		return m_impl->isRecording;
	}

	/*!
	* \brief Starts the record
	*
	* \remark Produces a NazaraError with NAZARA_AUDIO_SAFE defined if microphone creation failed
	*/

	void Microphone::Record()
	{
		#if NAZARA_AUDIO_SAFE
		if (!m_impl)
		{
			NazaraError("Microphone could not be created");
			return;
		}
		#endif

		if (!m_impl->isRecording)
		{
			m_impl->isRecording = true;
			m_impl->thread = Thread(&Microphone::MicrophoneThread, this);
		}
	}

	/*!
	* \brief Stops the record
	*
	* \remark Produces a NazaraError with NAZARA_AUDIO_SAFE defined if microphone creation failed
	*/

	void Microphone::Stop()
	{
		#if NAZARA_AUDIO_SAFE
		if (!m_impl)
		{
			NazaraError("Microphone could not be created");
			return;
		}
		#endif

		if (IsRecording())
		{
			m_impl->isRecording = false;
			m_impl->thread.Join();
		}
	}

	/*!
	* \brief Gets the number of bytes needed by the format
	* \return Number of bytes
	*
	* \param recordingFormat Quality of the format
	*
	* \remark Produces a NazaraInternalError if format is invalid
	*/

	UInt32 Microphone::GetFormatSize(RecordingFormat recordingFormat)
	{
		switch (recordingFormat)
		{
			case RecordingFormat_Mono8:
				return 1 * sizeof(UInt8);
			case RecordingFormat_Mono16:
				return 1 * sizeof(UInt16);
			case RecordingFormat_Stereo8:
				return 2 * sizeof(UInt8);
			case RecordingFormat_Stereo16:
				return 2 * sizeof(UInt16);
			default:
				NazaraInternalError("Invalid recording format (0x" + String::Number(recordingFormat, 16) + ')');
				return 0;
		}
	}

	/*!
	* \brief Thread function for the microphone
	*/

	void Microphone::MicrophoneThread()
	{
		m_impl->chunkSamples.Clear();
		alcCaptureStart(m_impl->captureDevice);

		// Recording loop (Filling samples as recording)
		while (IsRecording())
		{
			ALint samples;

			Nz::LockGuard lock(m_impl->bufferLock);
			alcGetIntegerv(m_impl->captureDevice, ALC_CAPTURE_SAMPLES, sizeof(ALint), &samples);
			if (samples == 0)
			{
				lock.Unlock();
				continue;
			}

			UInt64 oldSize = m_impl->processedSamples;
			m_impl->processedSamples += samples * GetFormatSize(m_impl->format);
			m_impl->chunkSamples.Resize(m_impl->processedSamples);
			alcCaptureSamples(m_impl->captureDevice, &m_impl->chunkSamples[oldSize], samples);
			lock.Unlock();

			// We go back to sleep
			Thread::Sleep(100);
		}

		alcCaptureStop(m_impl->captureDevice);
	}
}
