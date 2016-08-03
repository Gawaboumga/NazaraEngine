#include "AcidRain.hpp"

#include "Core/StateContext.hpp"
#include "Core/URL.hpp"

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
				Nz::SparsePtr<Nz::Vector3f> positionPtr = mapper.GetComponentPtr<Nz::Vector3f>(Nz::ParticleComponent_Position);
				Nz::SparsePtr<Nz::Vector3f> velocityPtr = mapper.GetComponentPtr<Nz::Vector3f>(Nz::ParticleComponent_Velocity);
				Nz::SparsePtr<float> lifePtr = mapper.GetComponentPtr<float>(Nz::ParticleComponent_Life);

				for (unsigned int i = startId; i <= endId; ++i)
				{
					Nz::Vector3f& position = positionPtr[i];
					Nz::Vector3f& velocity = velocityPtr[i];
					position += velocity * elapsedTime;
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
				time += elapsedTime;
				if (time >= 5.f)
				{
					system.GenerateParticles(1);
					time -= 5.f;
				}
			}

		private:

			void SetupParticles(Nz::ParticleMapper& mapper, unsigned int count) const override
			{
			}

			mutable float time = 0.f;
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

					particlePosition = Nz::Vector3f::Zero() + Nz::Vector3f::Unit() * 10.f;
					particleVelocity = Nz::Vector3f(1.f, 1.f, 0.f);
					particleLife = 10.f;
				}
			}
	};

	class TestParticleRenderer : public Nz::ParticleRenderer
	{
		public:

			TestParticleRenderer()
			{
				m_material = Nz::MaterialLibrary::Get("Basic2D");
				m_material->SetDiffuseMap(URL::GetImage(1));

				for (auto& declaration : m_declaration)
				{
					declaration.position = Nz::Vector3f::Zero();
					declaration.color = Nz::Color::Black;
					declaration.uv = Nz::Vector2f::Unit();
				}
			}

			void Render(const Nz::ParticleGroup& system, const Nz::ParticleMapper& mapper, unsigned int startId, unsigned int endId, Nz::AbstractRenderQueue* renderQueue) override
			{
				auto positionPtr = mapper.GetComponentPtr<Nz::Vector3f>(Nz::ParticleComponent_Position);
				for (unsigned int i = startId; i <= endId; ++i)
				{
					const Nz::Vector3f& particlePosition = positionPtr[i];
					m_declaration[i - startId].position = particlePosition;
				}
				renderQueue->AddSprites(10, m_material.Get(), m_declaration.data(), endId - startId);
			}

		private:
			Nz::MaterialRef m_material;
			std::array<Nz::VertexStruct_XYZ_Color_UV, 10> m_declaration;
	};

	AcidRain::AcidRain(StateContext& context)
	{
		m_rainEntity = context.world.CreateEntity();

		m_particleController = std::make_unique<TestParticleController>();
		m_particleEmitter = std::make_unique<TestParticleEmitter>();
		m_particleGenerator = std::make_unique<TestParticleGenerator>();

		auto& particleGroupComponent = m_rainEntity->AddComponent<Ndk::ParticleGroupComponent>(10, Nz::ParticleLayout_Sprite);
		particleGroupComponent.AddController(m_particleController.get());
		particleGroupComponent.AddEmitter(m_particleEmitter.get());
		particleGroupComponent.AddGenerator(m_particleGenerator.get());
		m_particleRenderer = std::make_unique<TestParticleRenderer>();
		particleGroupComponent.SetRenderer(m_particleRenderer.get());

		auto& particleEmitterComponent = m_rainEntity->AddComponent<Ndk::ParticleEmitterComponent>(&particleGroupComponent);
		particleEmitterComponent.SetActive();
		auto& nodeComponent = m_rainEntity->AddComponent<Ndk::NodeComponent>();
		nodeComponent.SetPosition(30.f, 30.f, 20.f);
	}

	AcidRain::~AcidRain()
	{
		m_particleController.release();
		m_particleEmitter.release();
		m_particleGenerator.release();
		m_particleRenderer.release();
		m_particleGroup.release();
	}

	void AcidRain::Update(float elapsedTime)
	{
	}

}
