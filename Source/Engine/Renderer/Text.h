#pragma once
#include "Font.h"
#include "Core/Math/Color.h"
#include <string>
#include <memory>

struct SDL_Texture;

namespace kiko
{
	class Renderer;

	class Text
	{
	public:
		//<create a default constructor>
		Text() = default;
		//	Text(<a shared pointer of Font type> font) : m_font{ font } { }
		Text(std::shared_ptr<Font> font) : m_font{ font } {}
		~Text();
		void Create(Renderer& renderer, const std::string& text, const Color& color);
		void Draw(Renderer& renderer, int x, int y);
		void Draw(Renderer& renderer, const class Transform& transform);
	private:
		//<a shared pointer of Font type> m_font;
		std::shared_ptr<Font> m_font;
		SDL_Texture* m_texture = nullptr;
	};
}