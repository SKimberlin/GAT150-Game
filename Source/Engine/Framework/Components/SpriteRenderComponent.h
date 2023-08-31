#pragma once
#include "RenderComponent.h"
#include "Renderer/Texture.h"
#include "Framework/Factory.h"

namespace kiko
{
	class SpriteRenderComponent : public RenderComponent
	{
	public:
		CLASS_DECLARATION(SpriteRenderComponent)

		bool Initialize() override;
		void Update(float dt) override;
		void Draw(class Renderer& renderer) override;

	public:
		std::string textureName;
		res_t<Texture> m_texture;
		Rect source;
		bool flipH = false;
		vec2 origin{ 0.5f, 0.5f };
	};
}