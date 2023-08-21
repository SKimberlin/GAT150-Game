#pragma once
#include "Framework/Actor.h"
#include "Framework/Components/PhysicsComponent.h"

class Player : public kiko::Actor
{
public:
	Player(float speed, float turnRate, const kiko::Transform& transform ) :
		Actor{ transform },
		m_speed{ speed },
		m_turnRate{ turnRate }
	{
		m_burstWeaponTime = 10.0f;
		m_burstWeaponTimer = m_burstWeaponTime;
	}

	bool Initialize() override;
	void OnDestroy() override;

	void Update(float dt) override;
	void OnCollision(Actor* other) override;

private:
	float m_speed = 0;
	float m_turnRate = 0;
	float m_health = 100;
	float m_burstWeaponTimer = 0.0f;
	float m_burstWeaponTime = 0.0f;

	kiko::PhysicsComponent* m_physicsComponent = nullptr;
};
