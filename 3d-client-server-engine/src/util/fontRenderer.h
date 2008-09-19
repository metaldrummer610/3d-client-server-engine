#ifndef _FONT_RENDERER_H
#define _FONT_RENDERER_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <stdarg.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_opengl.h>
#include "fileLoader.h"

class FontRenderer {
private:
	int nextpoweroftwo(int x);
	int round(double x);
	void glEnable2D();
	void glDisable2D();
	void drawText(char *text, TTF_Font *font, SDL_Color color, SDL_Rect *location);
	TTF_Font* font;
	int fontSize;
	SDL_Color color;
	SDL_Rect position;
	std::string fontpath;

	FileLoader fileLoader;
	std::map<std::string, std::string> properties;
public:
	FontRenderer();
	~FontRenderer();
	void glPrint(int x, int y, const char *fmt, ...);
	int getFontSize();
	void setFontSize(int i);
};

#endif
