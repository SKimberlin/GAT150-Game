#include "Enemy.h"

#include "Framework/Scene.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
#include "Framework/Components/SpriteAnimRenderComponent.h"
#include "Player.h"


namespace kiko
{
	CLASS_DEFINITION(Enemy)
		bool Enemy::Initialize()
	{
		Actor::Initialize();

		// cache off
		m_physicsComponent = GetComponent<kiko::PhysicsComponent>();
		m_spriteAnimRenderComponent = GetComponent<SpriteAnimRenderComponent>();

		return true;
	}

	void Enemy::OnDestroy()
	{
	}

	void Enemy::Update(float dt)
	{
		Actor::Update(dt);
		float dir = 0.0f;
		vec2 velocity = m_physicsComponent->velocity;

		// movement
		kiko::vec2 forward = kiko::vec2{ 1, 0 };
		Player* player = m_scene->GetActor<Player>();
		if (player && player->transform.position.Distance(this->transform.position) < 400.0f)
		{
			kiko::vec2 direction = player->transform.position - transform.position;
			dir = (direction.x > 0) ? 1.0f : -1.0f;
			m_spriteAnimRenderComponent->flipH = (velocity.x < -0.1);
			
		}
		if (std::fabs(velocity.x) > 0.2f)
		{
			m_spriteAnimRenderComponent->SetSequence("run");
		}
		else
		{
			m_spriteAnimRenderComponent->SetSequence("idle");
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