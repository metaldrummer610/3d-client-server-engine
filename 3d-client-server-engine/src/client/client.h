#ifndef _CLIENT_H
#define _CLIENT_H
#include <enet/enet.h>
#include <vector>
#include <map>
#include <stack>
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_opengl.h>
#include "../model/modelFactory.h"
#include "../util/fileLoader.h"
#include "../util/fontRenderer.h"
#include "../util/eventStack.h"

class Client {
private:
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	ENetHost* client;
	std::map<int, AbstractModel*> modelList;
	EventStack events;
	AbstractModel* player;
	ModelFactory modelFactory;
	FontRenderer fontFactory;
	SDL_Surface *screen;
	ENetPeer* peer;
	ENetAddress address;
	ENetEvent event;
	bool isActive;
	int videoFlags;
	int bpp;

	GLint T0;
	GLint Frames;
	GLint totalFrames;
	GLfloat seconds;
	GLfloat fps;
	bool renderFPS;
	bool lightOn;

	FileLoader fileLoader;
	std::map<std::string, std::string> properties;

	void getFPS();

	void addEventToStack(const char* x, ...);
	void displayEvents();

public:
	void init();
	void deinit();
	void mainLoop();
	void render();
	void handlePacket(ENetPacket *p);
	int resizeWindow(int width, int height);
	void handleKeyPress(SDL_keysym *keysym);
	void processSdlEvents();
	void sendPacket(string s);
	void sdl_openglInit(int width, int height);
};
#endif
