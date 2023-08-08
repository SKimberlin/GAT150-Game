#pragma once
#include "Core/Core.h"
#include "Renderer/Model.h"
#include "Renderer/ModelManager.h"
#include "Audio/AudioSystem.h"
#include "Components/SpriteComponent.h"
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

		virtual void Update(float dt);
		virtual void Draw(kiko::Renderer& renderer);

		void AddComponent(std::unique_ptr<Component> component);
		template<typename T>
		T* GetComponent();

		float GetRadius() { return 30.0f; }
		virtual void OnCollision(Actor* other) {}

		void SetLifespan(float lifespan) { m_lifespan = lifespan; }

		friend class Scene;

		class Scene* m_scene = nullptr;

		class Game* m_game = nullptr;

		kiko::Transform m_transform;
		std::string m_tag;

	protected:
		std::vector<std::unique_ptr<Component>> m_components;

		bool m_destroyed = false;
		float m_lifespan = -1.0f;

	};

	template<typename T>
	inline T* Actor::GetComponent()
	{
		for (auto& component : m_components)
		{
			T* result = dynamic_cast<T*>(component.get());
			if (result) return result;

		}
		return nullptr;
	}
}