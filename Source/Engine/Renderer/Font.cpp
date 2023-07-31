#include "Font.h"
#include <SDL2-2.28.0/include/SDL_ttf.h>
namespace kiko
{
	Font::Font(const std::string& filename, int fontSize)
	{
		//<call Load(filename, fontSize) to loadand create font>
		Load(filename, fontSize);
	}
	Font::~Font()
	{
		//<if m_ttfFont not null, close font(TTF_CloseFont)>
		if (m_ttfFont) TTF_CloseFont(m_ttfFont);
	}
	void Font::Load(const std::string& filename, int fontSize)
	{
		/*<call TTF_OpenFont>
			<use filename.c_str() to get the c - style string>
			<assign the return value of TTF_OpenFont to m_ttfFont>*/
		m_ttfFont = TTF_OpenFont(filename.c_str(), fontSize);
	}
}