#ifndef _CLIENT_H
#define _CLIENT_H
#include <enet/enet.h>
#include <vector>
#include <map>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include "../model/modelFactory.h"
#include "../util/textFactory.h"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

class Client {
private:
	ENetHost* client;
	std::map<int, AbstractModel*> modelList;
	AbstractModel* player;
	ModelFactory modelFactory;
	TextFactory textFactory;
	SDL_Surface *screen;
	ENetPeer* peer;
	ENetAddress address;
	ENetEvent event;
	bool isActive;
	int videoFlags;
	int bpp;
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
