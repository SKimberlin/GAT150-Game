#pragma once
#include "Framework/Actor.h"
#include "Framework/Components/PhysicsComponent.h"

namespace kiko
{
	class Player : public Actor
	{
	public:
		CLASS_DECLARATION(Player)

		bool Initialize() override;
		void OnDestroy() override;

		void Update(float dt) override;
		void OnCollisionEnter(Actor* other) override;


	public:
		float speed = 0;
		float turnRate = 0;
		float health = 100;
		float m_burstWeaponTimer = 0.0f;
		float m_burstWeaponTime = 0.0f;

		kiko::PhysicsComponent* m_physicsComponent = nullptr;
	};
}