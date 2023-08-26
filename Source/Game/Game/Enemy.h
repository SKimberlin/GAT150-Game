#pragma once
#include "Framework/Actor.h"
#include "Framework/Components/PhysicsComponent.h"

namespace kiko
{
	class Enemy : public Actor
	{
	public:
		CLASS_DECLARATION(Enemy)
		
		bool Initialize() override;
		void OnDestroy() override;
		void Update(float dt) override;
		void OnCollisionEnter(Actor* other) override;

		void SetFireTime(float fireTime) { fireRate = fireTime; }

	public:
		PhysicsComponent* m_physicsComponent = nullptr;

		float speed = 0;
		float turnRate = 0;

		float fireRate = 0;
		float fireTimer = 0;
	};
}