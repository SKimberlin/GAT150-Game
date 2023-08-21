#include "Enemy.h"
#include "Player.h"
#include "Weapon.h"
#include "SpaceGame.h"
#include "Framework/Scene.h"
#include "Renderer/Renderer.h"
#include "Framework/Emitter.h"

void Enemy::Update(float dt)
{
	Actor::Update(dt);

	kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(transform.rotation);
	Player* player = m_scene->GetActor<Player>();
	if (player) 
	{
		kiko::vec2 direction = player->transform.position - transform.position;

		// turn towards player
		float turnAngle = kiko::vec2::SignedAngle(forward, direction.Normalized());
		transform.rotation += turnAngle * dt * 2;
		
		// check if player is in front
		if (std::fabs(turnAngle) < kiko::DegreesToRadians(30.0f))
		{
			if (m_fireTimer <= 0)
			{
				/*kiko::Transform weaponTransform{ transform.position, transform.rotation, 0.5f };
				std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(400.0f, weaponTransform );
				weapon->tag = "EnemyBullet";
				m_scene->Add(std::move(weapon));
				m_fireTimer = m_fireTime;*/
			}
		}
	}


	transform.position += forward * m_speed * kiko::g_time.GetDeltaTime();
	transform.position.x = kiko::Wrap(transform.position.x, (float)kiko::g_renderer.GetWidth());
	transform.position.y = kiko::Wrap(transform.position.y, (float)kiko::g_renderer.GetHeight());

	m_fireTimer -= dt;
	
}

void Enemy::OnCollision(Actor* other)
{
	if (other->tag == "PlayerBullet")
	{
		m_game->AddPoints(100);
		kiko::EmitterData data;
		data.burst = true;
		data.burstCount = 100;
		data.spawnRate = 200;
		data.angle = 0;
		data.angleRange = kiko::Pi;
		data.lifetimeMin = 0.5f;
		data.lifetimeMin = 1.5f;
		data.speedMin = 50;
		data.speedMax = 250;
		data.damping = 0.5f;
		data.color = kiko::Color{ 1, 0, 0, 1 };
		kiko::Transform transform{ transform.position, 0, 1 };
		auto emitter = std::make_unique<kiko::Emitter>(transform, data);
		emitter->SetLifespan(1.0f);
		m_scene->Add(std::move(emitter));

		kiko::g_audioSystem.PlayOneShot("EnemyDeath");

		destroyed = true;

	}
}
