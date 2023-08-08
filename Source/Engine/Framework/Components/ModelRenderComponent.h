#pragma once

#include "Renderer/Model.h"
#include "RenderComponent.h"

namespace kiko
{
	class ModelRenderComponent : public RenderComponent
	{
	public:
		void Update(float dt) override;
		void Draw(class Renderer& renderer) override;

	public:
		res_t<Texture> m_texture;
	};
}