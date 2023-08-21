#pragma once
#include "Framework/Actor.h"


namespace kiko
{
	class Weapon : public Actor
	{
	public:
		CLASS_DECLARATION(Weapon)

		bool Initialize() override;
		void Update(float dt) override;
		void OnCollision(Actor* actor) override;

	private:
		float speed = 0;
		float m_turnRate = 0;
	};
}