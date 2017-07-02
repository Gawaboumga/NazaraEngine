// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Physics 2D module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Physics2D/PhysWorld2D.hpp>
#include <chipmunk/chipmunk.h>
#include <Nazara/Physics2D/Debug.hpp>

namespace Nz
{
	inline Vector2f FromCoordinates(const cpVect& position)
	{
		return Vector2f(static_cast<float>(position.x), -static_cast<float>(position.y));
	}

	inline cpVect ToCoordinates(const Nz::Vector2f& position)
	{
		return cpv(position.x, -position.y);
	}

	PhysWorld2D::PhysWorld2D() :
	m_stepSize(0.005f),
	m_timestepAccumulator(0.f)
	{
		m_handle = cpSpaceNew();
		cpSpaceSetUserData(m_handle, this);
	}

	PhysWorld2D::~PhysWorld2D()
	{
		cpSpaceFree(m_handle);
	}

	Vector2f PhysWorld2D::GetGravity() const
	{
		cpVect gravity = cpSpaceGetGravity(m_handle);
		return FromCoordinates(gravity);
	}

	cpSpace* PhysWorld2D::GetHandle() const
	{
		return m_handle;
	}

	float PhysWorld2D::GetStepSize() const
	{
		return m_stepSize;
	}

	bool PhysWorld2D::NearestBodyQuery(const Vector2f& from, float maxDistance, Nz::UInt32 collisionGroup, Nz::UInt32 categoryMask, Nz::UInt32 collisionMask, RigidBody2D** nearestBody)
	{
		cpShapeFilter filter = cpShapeFilterNew(collisionGroup, categoryMask, collisionMask);

		if (cpShape* shape = cpSpacePointQueryNearest(m_handle, ToCoordinates(from), maxDistance, filter, nullptr))
		{
			if (nearestBody)
				*nearestBody = static_cast<Nz::RigidBody2D*>(cpShapeGetUserData(shape));

			return true;
		}
		else
			return false;
	}

	bool PhysWorld2D::NearestBodyQuery(const Vector2f& from, float maxDistance, Nz::UInt32 collisionGroup, Nz::UInt32 categoryMask, Nz::UInt32 collisionMask, NearestQueryResult* result)
	{
		cpShapeFilter filter = cpShapeFilterNew(collisionGroup, categoryMask, collisionMask);

		if (result)
		{
			cpPointQueryInfo queryInfo;

			if (cpShape* shape = cpSpacePointQueryNearest(m_handle, ToCoordinates(from), maxDistance, filter, &queryInfo))
			{
				result->closestPoint.Set(FromCoordinates(queryInfo.point));
				result->distance = float(queryInfo.distance);
				result->fraction.Set(FromCoordinates(queryInfo.gradient));
				result->nearestBody = static_cast<Nz::RigidBody2D*>(cpShapeGetUserData(shape));

				return true;
			}
			else
				return false;
		}
		else
		{
			if (cpShape* shape = cpSpacePointQueryNearest(m_handle, ToCoordinates(from), maxDistance, filter, nullptr))
				return true;
			else
				return false;
		}
	}

	bool PhysWorld2D::RaycastQuery(const Nz::Vector2f& from, const Nz::Vector2f& to, float radius, Nz::UInt32 collisionGroup, Nz::UInt32 categoryMask, Nz::UInt32 collisionMask, std::vector<RaycastHit>* hitInfos)
	{
		using ResultType = decltype(hitInfos);

		auto callback = [](cpShape* shape, cpVect point, cpVect normal, cpFloat alpha, void* data)
		{
			ResultType results = static_cast<ResultType>(data);

			RaycastHit hitInfo;
			hitInfo.fraction = float(alpha);
			hitInfo.hitNormal.Set(FromCoordinates(normal));
			hitInfo.hitPos.Set(FromCoordinates(point));
			hitInfo.nearestBody = static_cast<Nz::RigidBody2D*>(cpShapeGetUserData(shape));

			results->emplace_back(std::move(hitInfo));
		};

		cpShapeFilter filter = cpShapeFilterNew(collisionGroup, categoryMask, collisionMask);

		std::size_t previousSize = hitInfos->size();
		cpSpaceSegmentQuery(m_handle, ToCoordinates(from), ToCoordinates(to), radius, filter, callback, hitInfos);

		return hitInfos->size() != previousSize;
	}

	bool PhysWorld2D::RaycastQueryFirst(const Nz::Vector2f& from, const Nz::Vector2f& to, float radius, Nz::UInt32 collisionGroup, Nz::UInt32 categoryMask, Nz::UInt32 collisionMask, RaycastHit* hitInfo)
	{
		cpShapeFilter filter = cpShapeFilterNew(collisionGroup, categoryMask, collisionMask);

		if (hitInfo)
		{
			cpSegmentQueryInfo queryInfo;

			if (cpShape* shape = cpSpaceSegmentQueryFirst(m_handle, ToCoordinates(from), ToCoordinates(to), radius, filter, &queryInfo))
			{
				hitInfo->fraction = float(queryInfo.alpha);
				hitInfo->hitNormal.Set(FromCoordinates(queryInfo.normal));
				hitInfo->hitPos.Set(FromCoordinates(queryInfo.point));
				hitInfo->nearestBody = static_cast<Nz::RigidBody2D*>(cpShapeGetUserData(queryInfo.shape));

				return true;
			}
			else
				return false;
		}
		else
		{
			if (cpShape* shape = cpSpaceSegmentQueryFirst(m_handle, ToCoordinates(from), ToCoordinates(to), radius, filter, nullptr))
				return true;
			else
				return false;
		}
	}

	void PhysWorld2D::RegionQuery(const Nz::Rectf& boundingBox, Nz::UInt32 collisionGroup, Nz::UInt32 categoryMask, Nz::UInt32 collisionMask, std::vector<Nz::RigidBody2D*>* bodies)
	{
		using ResultType = decltype(bodies);

		auto callback = [] (cpShape* shape, void* data)
		{
			ResultType results = static_cast<ResultType>(data);
			results->push_back(static_cast<Nz::RigidBody2D*>(cpShapeGetUserData(shape)));
		};

		cpShapeFilter filter = cpShapeFilterNew(collisionGroup, categoryMask, collisionMask);
		cpSpaceBBQuery(m_handle, cpBBNew(boundingBox.x, -(boundingBox.y + boundingBox.height), boundingBox.x + boundingBox.width, -boundingBox.y), filter, callback, bodies);
	}

	void PhysWorld2D::RegisterCallbacks(unsigned int collisionId, const Callback& callbacks)
	{
		InitCallbacks(cpSpaceAddWildcardHandler(m_handle, collisionId), callbacks);
	}

	void PhysWorld2D::RegisterCallbacks(unsigned int collisionIdA, unsigned int collisionIdB, const Callback& callbacks)
	{
		InitCallbacks(cpSpaceAddCollisionHandler(m_handle, collisionIdA, collisionIdB), callbacks);
	}

	void PhysWorld2D::SetGravity(const Vector2f& gravity)
	{
		cpSpaceSetGravity(m_handle, ToCoordinates(gravity));
	}

	void PhysWorld2D::SetStepSize(float stepSize)
	{
		m_stepSize = stepSize;
	}

	void PhysWorld2D::Step(float timestep)
	{
		m_timestepAccumulator += timestep;

		while (m_timestepAccumulator >= m_stepSize)
		{
			OnPhysWorld2DPreStep(this);

			cpSpaceStep(m_handle, m_stepSize);

			OnPhysWorld2DPostStep(this);
			if (!m_rigidPostSteps.empty())
			{
				for (const auto& pair : m_rigidPostSteps)
				{
					for (const auto& step : pair.second.funcs)
						step(pair.first);
				}

				m_rigidPostSteps.clear();
			}

			m_timestepAccumulator -= m_stepSize;
		}
	}

	void PhysWorld2D::InitCallbacks(cpCollisionHandler* handler, const Callback& callbacks)
	{
		auto it = m_callbacks.emplace(handler, std::make_unique<Callback>(callbacks)).first;

		handler->userData = it->second.get();

		if (callbacks.startCallback)
		{
			handler->beginFunc = [](cpArbiter* arb, cpSpace* space, void *data) -> cpBool
			{
				cpBody* firstBody;
				cpBody* secondBody;
				cpArbiterGetBodies(arb, &firstBody, &secondBody);

				PhysWorld2D* world = static_cast<PhysWorld2D*>(cpSpaceGetUserData(space));
				RigidBody2D* firstRigidBody = static_cast<RigidBody2D*>(cpBodyGetUserData(firstBody));
				RigidBody2D* secondRigidBody = static_cast<RigidBody2D*>(cpBodyGetUserData(secondBody));

				const Callback* customCallbacks = static_cast<const Callback*>(data);
				if (customCallbacks->startCallback(*world, *firstRigidBody, *secondRigidBody, customCallbacks->userdata))
				{
					cpBool retA = cpArbiterCallWildcardBeginA(arb, space);
					cpBool retB = cpArbiterCallWildcardBeginB(arb, space);
					return retA && retB;
				}
				else
					return cpFalse;
			};
		}

		if (callbacks.endCallback)
		{
			handler->separateFunc = [](cpArbiter* arb, cpSpace* space, void *data)
			{
				cpBody* firstBody;
				cpBody* secondBody;
				cpArbiterGetBodies(arb, &firstBody, &secondBody);

				PhysWorld2D* world = static_cast<PhysWorld2D*>(cpSpaceGetUserData(space));
				RigidBody2D* firstRigidBody = static_cast<RigidBody2D*>(cpBodyGetUserData(firstBody));
				RigidBody2D* secondRigidBody = static_cast<RigidBody2D*>(cpBodyGetUserData(secondBody));

				const Callback* customCallbacks = static_cast<const Callback*>(data);
				customCallbacks->endCallback(*world, *firstRigidBody, *secondRigidBody, customCallbacks->userdata);

				cpArbiterCallWildcardSeparateA(arb, space);
				cpArbiterCallWildcardSeparateB(arb, space);
			};
		}

		if (callbacks.preSolveCallback)
		{
			handler->preSolveFunc = [](cpArbiter* arb, cpSpace* space, void *data) -> cpBool
			{
				cpBody* firstBody;
				cpBody* secondBody;
				cpArbiterGetBodies(arb, &firstBody, &secondBody);

				PhysWorld2D* world = static_cast<PhysWorld2D*>(cpSpaceGetUserData(space));
				RigidBody2D* firstRigidBody = static_cast<RigidBody2D*>(cpBodyGetUserData(firstBody));
				RigidBody2D* secondRigidBody = static_cast<RigidBody2D*>(cpBodyGetUserData(secondBody));

				const Callback* customCallbacks = static_cast<const Callback*>(data);
				if (customCallbacks->preSolveCallback(*world, *firstRigidBody, *secondRigidBody, customCallbacks->userdata))
				{
					cpBool retA = cpArbiterCallWildcardPreSolveA(arb, space);
					cpBool retB = cpArbiterCallWildcardPreSolveB(arb, space);
					return retA && retB;
				}
				else
					return cpFalse;
			};
		}

		if (callbacks.postSolveCallback)
		{
			handler->postSolveFunc = [](cpArbiter* arb, cpSpace* space, void *data)
			{
				cpBody* firstBody;
				cpBody* secondBody;
				cpArbiterGetBodies(arb, &firstBody, &secondBody);

				PhysWorld2D* world = static_cast<PhysWorld2D*>(cpSpaceGetUserData(space));
				RigidBody2D* firstRigidBody = static_cast<RigidBody2D*>(cpBodyGetUserData(firstBody));
				RigidBody2D* secondRigidBody = static_cast<RigidBody2D*>(cpBodyGetUserData(secondBody));

				const Callback* customCallbacks = static_cast<const Callback*>(data);
				customCallbacks->postSolveCallback(*world, *firstRigidBody, *secondRigidBody, customCallbacks->userdata);

				cpArbiterCallWildcardPostSolveA(arb, space);
				cpArbiterCallWildcardPostSolveB(arb, space);
			};
		}
	}

	void PhysWorld2D::OnRigidBodyMoved(RigidBody2D* oldPointer, RigidBody2D* newPointer)
	{
		auto it = m_rigidPostSteps.find(oldPointer);
		if (it == m_rigidPostSteps.end())
			return; //< Shouldn't happen

		m_rigidPostSteps.emplace(std::make_pair(newPointer, std::move(it->second)));
		m_rigidPostSteps.erase(oldPointer);
	}

	void PhysWorld2D::OnRigidBodyRelease(RigidBody2D* rigidBody)
	{
		m_rigidPostSteps.erase(rigidBody);
	}

	void PhysWorld2D::RegisterPostStep(RigidBody2D* rigidBody, PostStep&& func)
	{
		// If space isn't locked, no need to wait
		if (!cpSpaceIsLocked(m_handle))
		{
			func(rigidBody);
			return;
		}

		auto it = m_rigidPostSteps.find(rigidBody);
		if (it == m_rigidPostSteps.end())
		{
			PostStepContainer postStep;
			postStep.onMovedSlot.Connect(rigidBody->OnRigidBody2DMove, this, &PhysWorld2D::OnRigidBodyMoved);
			postStep.onReleaseSlot.Connect(rigidBody->OnRigidBody2DRelease, this, &PhysWorld2D::OnRigidBodyRelease);

			it = m_rigidPostSteps.insert(std::make_pair(rigidBody, std::move(postStep))).first;
		}

		it->second.funcs.emplace_back(std::move(func));
	}
}
