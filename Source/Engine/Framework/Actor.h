#pragma once
#include "Core/Core.h"
#include "Renderer/Model.h"
#include "Renderer/ModelManager.h"
#include "Audio/AudioSystem.h"
#include <memory>

namespace kiko
{

	class Actor
	{
	public:
		Actor() = default;
		Actor(const kiko::Transform& transform) :
			m_transform{ transform }
		{}
		Actor(const kiko::Transform& transform, std::shared_ptr<Model> model) :
			m_transform{ transform },
			m_model{ model }
		{}


		virtual void Update(float dt);
		virtual void Draw(kiko::Renderer& renderer);

		float GetRadius() { return (m_model) ? m_model->GetRadius() * m_transform.scale : 0; }
		virtual void OnCollision(Actor* other) {}
		
		void AddForce(const vec2 force) { m_velocity += force; }
		void SetDamping(float damping) { m_damping = damping; }

		void SetLifespan(float lifespan) { m_lifespan = lifespan; }

		friend class Scene;

		class Scene* m_scene = nullptr;

		class Game* m_game = nullptr;

		kiko::Transform m_transform;
		std::string m_tag;

	protected:
		bool m_destroyed = false;
		float m_lifespan = -1.0f;

		std::shared_ptr<Model> m_model;

		vec2 m_velocity;
		float m_damping = 0;
	};
}