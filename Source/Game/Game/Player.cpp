#include "Player.h"
#include "Weapon.h"
#include "SpaceGame.h"
#include "Framework/Scene.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"

void Player::Update(float dt)
{
	Actor::Update(dt);

	// movement
	float rotate = 0;
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) rotate = -1;
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) rotate = 1;
	m_transform.rotation += rotate * m_turnRate * kiko::g_time.GetDeltaTime();

	float thrust = 0;
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_W)) thrust = 1;

	kiko::vec2 forward = kiko::vec2{0, -1}.Rotate(m_transform.rotation);
	AddForce(forward * m_speed * thrust);

	//m_transform.position += forward * m_speed * thrust * kiko::g_time.GetDeltaTime();
	m_transform.position.x = kiko::Wrap(m_transform.position.x, (float)kiko::g_renderer.GetWidth());
	m_transform.position.y = kiko::Wrap(m_transform.position.y, (float)kiko::g_renderer.GetHeight());

	// fire weapon 1 shot
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && !kiko::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE))
	{
		//create weapon
		kiko::Transform transform{ m_transform.position, m_transform.rotation, 0.5f };
		std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>( 400.0f, transform, kiko::g_manager.Get("PlayerBullet.txt"));
		weapon->m_tag = "PlayerBullet";
		m_scene->Add(std::move(weapon));

		kiko::g_audioSystem.PlayOneShot("Laser_Shoot");
	}

	// fire ball of death
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_M) && !kiko::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_M) && (m_burstWeaponTimer <= 0))
	{
		for (float i = 0; i < 360; i += 10)
		{
			//create weapon
			kiko::Transform transform{ m_transform.position, kiko::DegreesToRadians(i), 0.5f };
			std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(400.0f, transform, kiko::g_manager.Get("PlayerBullet.txt"));
			weapon->m_tag = "PlayerBullet";
			m_scene->Add(std::move(weapon));
			m_burstWeaponTimer = m_burstWeaponTime;
			kiko::g_audioSystem.PlayOneShot("FireBall");
		}
	}


	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_T)) kiko::g_time.SetTimeScale(0.5f);
	else kiko::g_time.SetTimeScale(1.0f);

	if (m_health <= 0)
	{
		m_game->SetLives(m_game->GetLives() - 1);
		dynamic_cast<SpaceGame*>(m_game)->SetState(SpaceGame::eState::PlayerDeadStart);
		m_destroyed = true;
	}

	m_burstWeaponTimer -= dt;
}

void Player::OnCollision(Actor* other)
{
	if (other->m_tag == "EnemyBullet")
	{
		m_health -= 10;
	}
}
