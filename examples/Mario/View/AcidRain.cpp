#include "AcidRain.hpp"

#include "Core/StateContext.hpp"

#include <Nazara/Core/SparsePtr.hpp>
#include <Nazara/Graphics/AbstractRenderQueue.hpp>
#include <Nazara/Graphics/Material.hpp>
#include <Nazara/Graphics/ParticleGroup.hpp>
#include <Nazara/Graphics/ParticleMapper.hpp>

#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Components/ParticleGroupComponent.hpp>
#include <NDK/Components/ParticleEmitterComponent.hpp>
#include <NDK/World.hpp>

namespace SMB
{

	class TestParticleController : public Nz::ParticleController
	{
		public:

			// Be aware that the interval is [startId, endId] and NOT [startId, endId)
			void Apply(Nz::ParticleGroup& system, Nz::ParticleMapper& mapper, unsigned int startId, unsigned int endId, float elapsedTime) override
			{
				Nz::SparsePtr<float> lifePtr = mapper.GetComponentPtr<float>(Nz::ParticleComponent_Life);

				for (unsigned int i = startId; i <= endId; ++i)
				{
					float& particleLife = lifePtr[i];

					particleLife -= elapsedTime;
					if (particleLife <= 0.f)
						system.KillParticle(i);
				}
			}
	};

	class TestParticleEmitter : public Nz::ParticleEmitter
	{
		public:

			void Emit(Nz::ParticleGroup& system, float elapsedTime) const override
			{
				system.GenerateParticles(10);
			}

		private:

			void SetupParticles(Nz::ParticleMapper& mapper, unsigned int count) const override
			{
			}
	};

	class TestParticleGenerator : public Nz::ParticleGenerator
	{
		public:

			// Be aware that the interval is [startId, endId] and NOT [startId, endId)
			void Generate(Nz::ParticleGroup& system, Nz::ParticleMapper& mapper, unsigned int startId, unsigned int endId) override
			{
				Nz::SparsePtr<Nz::Vector3f> positionPtr = mapper.GetComponentPtr<Nz::Vector3f>(Nz::ParticleComponent_Position);
				Nz::SparsePtr<Nz::Vector3f> velocityPtr = mapper.GetComponentPtr<Nz::Vector3f>(Nz::ParticleComponent_Velocity);
				Nz::SparsePtr<float> lifePtr = mapper.GetComponentPtr<float>(Nz::ParticleComponent_Life);

				for (unsigned int i = startId; i <= endId; ++i)
				{
					Nz::Vector3f& particlePosition = positionPtr[i];
					Nz::Vector3f& particleVelocity = velocityPtr[i];
					float& particleLife = lifePtr[i];

					particlePosition = Nz::Vector3f::Zero() + Nz::Vector3f::UnitZ() * 10.f;
					particleVelocity = Nz::Vector3f::Zero();
					particleLife = 1.3f;
				}
			}
	};

	class TestParticleRenderer : public Nz::ParticleRenderer
	{
		public:

			void Render(const Nz::ParticleGroup& system, const Nz::ParticleMapper& mapper, unsigned int startId, unsigned int endId, Nz::AbstractRenderQueue* renderQueue) override
			{
				NazaraWarning("???");
				NazaraWarning(Nz::String::Number(startId) + " - " + Nz::String::Number(endId));
				auto positionPtr = mapper.GetComponentPtr<Nz::Vector3f>(Nz::ParticleComponent_Position);
				auto lifePtr = mapper.GetComponentPtr<const float>(Nz::ParticleComponent_Life);
				auto material = Nz::MaterialLibrary::Get("Basic2D");
				material->SetDiffuseMap("mario_tileset");
				renderQueue->AddBillboards(10, material.Get(), endId - startId, positionPtr, lifePtr);
			}
	};

	AcidRain::AcidRain(StateContext& context)
	{
		m_rainEntity = context.world.CreateEntity();

		m_particleController = std::make_unique<TestParticleController>();
		m_particleEmitter = std::make_unique<TestParticleEmitter>();
		m_particleGenerator = std::make_unique<TestParticleGenerator>();
		m_particleGroup = std::make_unique<Nz::ParticleGroup>(10, Nz::ParticleLayout_Sprite);
		m_particleGroup->AddController(m_particleController.get());
		m_particleGroup->AddEmitter(m_particleEmitter.get());
		m_particleGroup->AddGenerator(m_particleGenerator.get());

		m_particleRenderer = std::make_unique<TestParticleRenderer>();
		m_particleGroup->SetRenderer(m_particleRenderer.get());

		m_rainEntity->AddComponent<Ndk::ParticleEmitterComponent>(m_particleGroup.get());
		m_rainEntity->AddComponent<Ndk::ParticleGroupComponent>(m_particleGroup.get());
		auto& nodeComponent = m_rainEntity->AddComponent<Ndk::NodeComponent>();
		nodeComponent.SetPosition(Nz::Vector3f::Unit() * 10.f);
	}

	AcidRain::~AcidRain()
	{
		m_particleController.release();
		m_particleEmitter.release();
		m_particleGenerator.release();
		m_particleRenderer.release();
		m_particleGroup->SetRenderer(nullptr);
		m_particleGroup.release();
	}

	void AcidRain::Update(float elapsedTime)
	{
		m_particleGroup->Update(elapsedTime);
	}

}
