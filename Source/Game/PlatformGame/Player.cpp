#include "Player.h"

#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
#include "Framework/Framework.h"


namespace kiko
{
	CLASS_DEFINITION(Player)
		bool Player::Initialize()
	{
		Actor::Initialize();

		// cache off
		m_physicsComponent = GetComponent<PhysicsComponent>();
		m_spriteAnimRenderComponent = GetComponent<SpriteAnimRenderComponent>();

		return true;
	}

	void Player::OnDestroy()
	{
	}

	void Player::Update(float dt)
	{
		Actor::Update(dt);

		vec2 forward = kiko::vec2{ 1.0f, 0.0f };
		vec2 velocity = m_physicsComponent->velocity;
		bool onGround = (groundCount > 0);
		
		

		// movement
		float dir = 0.0f;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) dir = -1.0f;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) dir = 1.0f;

		
			
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_Z) && !g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_Z) && std::abs(dir) > 0)
		{
			m_spriteAnimRenderComponent->SetSequence("roll");
			forward = vec2{ 3.0f, 0.0f };
		}

		// jump
		if (onGround && kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && !kiko::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE))
		{
			m_physicsComponent->SetVelocity(vec2{ velocity.x, -1.0f * jump });
			m_spriteAnimRenderComponent->SetSequence("jump");
		}
		if (velocity.y > 0.2f && m_spriteAnimRenderComponent->GetSequenceName() != "fall")
		{
			m_spriteAnimRenderComponent->SetSequence("fall");
		}
		
		std::string currentSequenceName = m_spriteAnimRenderComponent->GetSequenceName();
		bool inAnimation = (currentSequenceName != "idle" && currentSequenceName != "run");

		// animation
		if (!inAnimation)
		{
			if (std::fabs(velocity.x) > 0.2f)
			{
				m_spriteAnimRenderComponent->flipH = (velocity.x < -0.1);
				m_spriteAnimRenderComponent->SetSequence("run");
			}
			else
			{
				m_spriteAnimRenderComponent->SetSequence("idle");
			}
		}
		if (m_spriteAnimRenderComponent->IsEndFrame())
		{
			if (currentSequenceName == "roll")
			{
				m_spriteAnimRenderComponent->SetSequence("idle");
			}
			if (currentSequenceName == "fall" && velocity.y < 0.2f)
			{
				m_spriteAnimRenderComponent->SetSequence("idle");
			}
			if (currentSequenceName == "jump")
			{
				m_spriteAnimRenderComponent->SetSequence("fall");
			}
		}
		


		m_physicsComponent->ApplyForce(forward * speed * dir * (onGround ? 1.0f : 0.5));
	}

	void Player::OnCollisionEnter(Actor* other)
	{
		if (other->tag == "Ground") groundCount++;
		if (other->tag == "Exit" && m_game->GetCoinCount() <= 0)
		{
			kiko::EventManager::Instance().DispatchEvent("OnLevelComplete", 0);
		}
		if (other->tag == "Enemy" && m_spriteAnimRenderComponent->GetSequenceName() != "roll")
		{
			kiko::EventManager::Instance().DispatchEvent("OnPlayerDead", 0);
		}
		if (other->tag == "Coin")
		{
			kiko::EventManager::Instance().DispatchEvent("OnCoinPickup", 0);
			other->destroyed = true;
		}
	}

	void Player::OnCollisionExit(Actor* other)
	{
		if (other->tag == "Ground") groundCount--;
	}

	void Player::Read(const json_t& value)
	{
		Actor::Read(value);
		READ_DATA(value, speed);
		READ_DATA(value, jump);

	}
}