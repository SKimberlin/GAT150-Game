#pragma once
#include "Framework/Actor.h"
#include "Framework/Components/PhysicsComponent.h"


namespace kiko
{
	class Enemy : public Actor
	{
	public:
		CLASS_DECLARATION(Enemy)

		bool Initialize() override;
		void OnDestroy() override;

		void Update(float dt) override;
		void OnCollisionEnter(Actor* other) override;
		void OnCollisionExit(Actor* other) override;


	public:
		float speed = 0;
		float health = 100;

		PhysicsComponent* m_physicsComponent = nullptr;
		class SpriteAnimRenderComponent* m_spriteAnimRenderComponent = nullptr;
	};
}