#include "Enemy.h"
#include "Player.h"
#include "Weapon.h"
#include "SpaceGame.h"
#include "Framework/Scene.h"
#include "Renderer/Renderer.h"
#include "Framework/Emitter.h"

namespace kiko
{
	CLASS_DEFINITION(Enemy)
	bool Enemy::Initialize()
	{
		Actor::Initialize();


		return true;
	}
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
			m_physicsComponent->ApplyTorque(turnAngle);

			// check if player is in front
			if (std::fabs(turnAngle) < kiko::DegreesToRadians(30.0f))
			{
				if (fireTimer <= 0)
				{
					/*kiko::Transform weaponTransform{ transform.position, transform.rotation, 0.5f };
					std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(400.0f, weaponTransform );
					weapon->tag = "EnemyBullet";
					m_scene->Add(std::move(weapon));
					m_fireTimer = m_fireTime;*/
				}
			}
		}
		

		m_physicsComponent->ApplyForce(forward * speed);

		transform.position += forward * speed * kiko::g_time.GetDeltaTime();
		transform.position.x = kiko::Wrap(transform.position.x, (float)kiko::g_renderer.GetWidth());
		transform.position.y = kiko::Wrap(transform.position.y, (float)kiko::g_renderer.GetHeight());

		fireTimer -= dt;

	}

	void Enemy::OnCollisionEnter(Actor* other)
	{
		if (other->tag == "PlayerBullet")
		{
			kiko::EventManager::Instance().DispatchEvent("OnAddPoints", 100);
			//m_game->AddPoints(100);
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
	void Enemy::OnDestroy()
	{

	}
	void Enemy::Read(const json_t& value)
	{
		Actor::Read(value);
		READ_DATA(value, speed);
		READ_DATA(value, turnRate);
		READ_DATA(value, fireRate);
	}
}