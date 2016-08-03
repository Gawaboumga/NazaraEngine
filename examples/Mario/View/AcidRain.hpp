#ifndef __ACIDRAIN_HPP__
#define __ACIDRAIN_HPP__

#include <Nazara/Graphics/ParticleController.hpp>
#include <Nazara/Graphics/ParticleEmitter.hpp>
#include <Nazara/Graphics/ParticleGenerator.hpp>
#include <Nazara/Graphics/ParticleGroup.hpp>
#include <Nazara/Graphics/ParticleRenderer.hpp>

#include <NDK/Entity.hpp>

#include <memory>

namespace SMB
{
	class StateContext;
}

namespace SMB
{

	class AcidRain
	{
		public:

			AcidRain(StateContext& stateContext);
			~AcidRain();

			void Update(float elapsedTime);

		private:

			std::unique_ptr<Nz::ParticleController> m_particleController;
			std::unique_ptr<Nz::ParticleEmitter> m_particleEmitter;
			std::unique_ptr<Nz::ParticleGenerator> m_particleGenerator;
			std::unique_ptr<Nz::ParticleGroup> m_particleGroup;
			std::unique_ptr<Nz::ParticleRenderer> m_particleRenderer;
			Ndk::EntityHandle m_rainEntity;
	};

}

#endif // __ACIDRAIN_HPP__
