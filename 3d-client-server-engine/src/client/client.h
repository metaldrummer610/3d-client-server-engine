#ifndef _CLIENT_H
#define _CLIENT_H
#include <enet/enet.h>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include "../model/abstractModel.h"
#include "../model/modelFactory.h"
//#include "../model/cube.h"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

class Client {
private:
	ENetHost* client;
	std::vector<AbstractModel*> modelList;
	AbstractModel* player;
	ModelFactory factory;
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
