#include "Enemy.h"

#include "Framework/Scene.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
#include "Player.h"


namespace kiko
{
	CLASS_DEFINITION(Enemy)
		bool Enemy::Initialize()
	{
		Actor::Initialize();

		// cache off
		m_physicsComponent = GetComponent<kiko::PhysicsComponent>();

		return true;
	}

	void Enemy::OnDestroy()
	{
	}

	void Enemy::Update(float dt)
	{
		Actor::Update(dt);
		float dir = 0;

		// movement
		kiko::vec2 forward = kiko::vec2{ 1, 0 };
		Player* player = m_scene->GetActor<Player>();
		if (player)
		{
			kiko::vec2 direction = player->transform.position - transform.position;
			dir = (direction.x > 0) ? 1 : -1;
		}

		m_physicsComponent->ApplyForce(forward * speed * dir);

	}

	void Enemy::OnCollisionEnter(Actor* other)
	{
		
	}

	void Enemy::OnCollisionExit(Actor* other)
	{
		
	}

	void Enemy::Read(const json_t& value)
	{
		Actor::Read(value);
		READ_DATA(value, speed);

	}
}