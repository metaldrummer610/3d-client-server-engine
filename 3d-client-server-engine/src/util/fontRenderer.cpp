#include "fontRenderer.h"
#include <iostream>
#include <sstream>

FontRenderer::FontRenderer() {
	properties = fileLoader.loadConfigFile();

	std::istringstream in(properties.find("FONT_COLOR_R")->second);
	in >> color.r;
	in.clear();

	in.str(properties.find("FONT_COLOR_G")->second);
	in >> color.g;
	in.clear();

	in.str(properties.find("FONT_COLOR_B")->second);

	fontpath = "resources/action_force_normal.ttf";
	fontSize = 12;

	TTF_Init();
	atexit(TTF_Quit);

	if (!(font = TTF_OpenFont(fontpath.c_str(), fontSize))) {
		std::cout << "Error loading font: " << TTF_GetError() << std::endl;
		exit(1);
	}
}

FontRenderer::~FontRenderer() {
	TTF_CloseFont(font);
}

int FontRenderer::getFontSize() {
	return fontSize;
}

void FontRenderer::setFontSize(int i) {
	fontSize = i;

	if (!(font = TTF_OpenFont(fontpath.c_str(), fontSize))) {
		std::cout << "Error loading font: " << TTF_GetError() << std::endl;
		exit(1);
	}
}

int FontRenderer::round(double x) {
	return (int) (x + 0.5);
}

int FontRenderer::nextpoweroftwo(int x) {
	double logbase2 = log(x) / log(2);
	return round(pow(2, ceil(logbase2)));
}

void FontRenderer::glPrint(int x, int y, const char *fmt, ...) {
	char text[256]; /* Holds our string */
	va_list ap; /* Pointer to our list of elements */

	/* If there's no text, do nothing */
	if (text == NULL)
		return;

	/* Parses The String For Variables */
	va_start(ap, fmt);
	/* Converts Symbols To Actual Numbers */
	vsprintf(text, fmt, ap);
	va_end(ap);

	glEnable2D();
	glDisable(GL_DEPTH_TEST);

	position.x = x;
	position.y = y;

	drawText(text, font, color, &position);

	glEnable(GL_DEPTH_TEST);
	glDisable2D();
}

void FontRenderer::drawText(char *text, TTF_Font *font, SDL_Color color,
		SDL_Rect *location) {
	SDL_Surface *initial;
	SDL_Surface *intermediary;
	SDL_Rect rect;
	int w, h;
	GLuint texture;

	/* Use SDL_TTF to render our text */
	initial = TTF_RenderText_Blended(font, text, color);

	/* Convert the rendered text to a known format */
	w = nextpoweroftwo(initial->w);
	h = nextpoweroftwo(initial->h);

	intermediary = SDL_CreateRGBSurface(0, w, h, 32,
#if SDL_BYTEORDER != SDL_LIL_ENDIAN // OpenGL RGBA masks
                               0x000000FF,
                               0x0000FF00,
                               0x00FF0000,
                               0xFF000000
#else
                               0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF
#endif
								);
	//0x00ff0000, 0x0000ff00,
	//0x000000ff, 0xff000000);

	SDL_BlitSurface(initial, 0, intermediary, 0);

	/* Tell GL about our new texture */
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE,
			intermediary->pixels);

	/* GL_NEAREST looks horrible, if scaled... */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/* prepare to render our texture */
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glColor3f(1.0f, 1.0f, 1.0f);

	/* Draw a quad at location */
	glBegin(GL_QUADS);
	/* Recall that the origin is in the lower-left corner
	 That is why the TexCoords specify different corners
	 than the Vertex coors seem to. */
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(location->x, location->y);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(location->x + w, location->y);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(location->x + w, location->y + h);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(location->x, location->y + h);
	glEnd();

	/* Bad things happen if we delete the texture before it finishes */
	glFinish();

	/* return the deltas in the unused w,h part of the rect */
	location->w = initial->w;
	location->h = initial->h;

	/* Clean up */
	SDL_FreeSurface(initial);
	SDL_FreeSurface(intermediary);
	glDeleteTextures(1, &texture);
}

void FontRenderer::glEnable2D() {
	int vPort[4];

	glGetIntegerv(GL_VIEWPORT, vPort);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0, vPort[2], 0, vPort[3], -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void FontRenderer::glDisable2D() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
