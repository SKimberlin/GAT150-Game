#include "Player.h"
#include "Weapon.h"
#include "SpaceGame.h"
#include "Framework/Scene.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
#include "Framework/Components/SpriteRenderComponent.h"
#include "Framework/Resource/ResourceManager.h"
#include "Framework/Components/PhysicsComponent.h"
#include "Framework/Components/CircleCollisionComponent.h"

namespace kiko
{
	CLASS_DEFINITION(Player)
	bool Player::Initialize()
	{
		Actor::Initialize();

		// cache off
		m_physicsComponent = GetComponent<kiko::PhysicsComponent>();
		auto collisionComponent = GetComponent<kiko::CollisionComponent>();
		if (collisionComponent)
		{
			auto renderComponent = GetComponent<kiko::RenderComponent>();
			if (renderComponent)
			{
				float scale = transform.scale;
				collisionComponent->m_radius = renderComponent->GetRadius() * scale;
			}

		}

		return true;
	}

	void Player::OnDestroy()
	{
	}

	void Player::Update(float dt)
	{
		Actor::Update(dt);

		// movement
		float rotate = 0;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) rotate = -1;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) rotate = 1;

		m_physicsComponent->ApplyTorque(rotate * turnRate);

		float thrust = 0;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_W)) thrust = 1;

		kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(transform.rotation);

		auto physicsComponent = m_physicsComponent;
		physicsComponent->ApplyForce(forward * speed * thrust);

		//m_transform.position += forward * m_speed * thrust * kiko::g_time.GetDeltaTime();
		transform.position.x = kiko::Wrap(transform.position.x, (float)kiko::g_renderer.GetWidth());
		transform.position.y = kiko::Wrap(transform.position.y, (float)kiko::g_renderer.GetHeight());

		// fire weapon 1 shot
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && !kiko::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE))
		{
			auto weapon = INSTANTIATE(Weapon, "Rocket")
			weapon->transform = { transform.position, transform.rotation, 1.0f };
			weapon->Initialize();
			m_scene->Add(std::move(weapon));

			//create weapon
			/*kiko::Transform weaponTransform{ transform.position, transform.rotation, 0.5f };
			std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>( 400.0f, weaponTransform );
			weapon->tag = "PlayerBullet";

			std::unique_ptr<kiko::SpriteRenderComponent> component = std::make_unique<kiko::SpriteRenderComponent>();
			component->m_texture = GET_RESOURCE(kiko::Texture, "Rocket.png", kiko::g_renderer);
			weapon->AddComponent(std::move(component));

			auto collisionComponent = std::make_unique<kiko::CircleCollisionComponent>();
			collisionComponent->m_radius = 20.0f;
			weapon->AddComponent(std::move(collisionComponent));

			weapon->Initialize();
			m_scene->Add(std::move(weapon));

			kiko::g_audioSystem.PlayOneShot("Laser_Shoot");*/
		}

		// fire ball of death
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_M) && !kiko::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_M) && (m_burstWeaponTimer <= 0))
		{
			for (float i = 0; i < 360; i += 10)
			{
				auto weapon = INSTANTIATE(Weapon, "Rocket")
				weapon->transform = { transform.position, transform.rotation, 1.0f };
				weapon->Initialize();
				m_scene->Add(std::move(weapon));

				//create weapon
				/*kiko::Transform weaponTransform{ transform.position, kiko::DegreesToRadians(i), 0.5f };
				std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(400.0f, weaponTransform);
				weapon->tag = "PlayerBullet";

				std::unique_ptr<kiko::SpriteRenderComponent> component = std::make_unique<kiko::SpriteRenderComponent>();
				component->m_texture = GET_RESOURCE(kiko::Texture, "Rocket.png", kiko::g_renderer);
				weapon->AddComponent(std::move(component));

				m_scene->Add(std::move(weapon));
				m_burstWeaponTimer = m_burstWeaponTime;
				kiko::g_audioSystem.PlayOneShot("FireBall");*/
			}
		}


		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_T)) kiko::g_time.SetTimeScale(0.5f);
		else kiko::g_time.SetTimeScale(1.0f);

		if (health <= 0)
		{
			kiko::EventManager::Instance().DispatchEvent("OnPlayerDead", 0);
			destroyed = true;
		}

		m_burstWeaponTimer -= dt;
	}

	void Player::OnCollisionEnter(Actor* other)
	{
		if (other->tag == "EnemyBullet")
		{
			health -= 10;
		}
	}

	void Player::Read(const json_t& value)
	{
		Actor::Read(value);
		READ_DATA(value, speed);
		READ_DATA(value, turnRate);
		
	}
}