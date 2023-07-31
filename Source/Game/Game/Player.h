#pragma once
#include "Framework/Actor.h"

class Player : public kiko::Actor
{
public:
	Player(float speed, float turnRate, const kiko::Transform& transform, std::shared_ptr<kiko::Model> model) :
		Actor{ transform, model },
		m_speed{ speed },
		m_turnRate{ turnRate }
	{
		m_burstWeaponTime = 10.0f;
		m_burstWeaponTimer = m_burstWeaponTime;
	}
	void Update(float dt) override;
	void OnCollision(Actor* other) override;

private:
	float m_speed = 0;
	float m_turnRate = 0;
	float m_health = 100;
	float m_burstWeaponTimer = 0.0f;
	float m_burstWeaponTime = 0.0f;
};
