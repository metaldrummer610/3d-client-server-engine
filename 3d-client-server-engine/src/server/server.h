#ifndef _SERVER_H
#define _SERVER_H
#include <enet/enet.h>
#include <map>
#include <string>
#include <SDL/SDL.h>
#include "../model/modelFactory.h"
#include "../util/fileLoader.h"

class Server {

private:
	ENetHost* server;
	ENetAddress address;
	std::map<int, AbstractModel*> modelList;
	ModelFactory factory;
	int peerNumber;

	void loadChangedModels();
	void addBackground();

public:
	void init();
	void deinit();
	void mainLoop();
	void sendModels(ENetPeer *p);
	void addModelToList(ENetPeer *p);
	AbstractModel* createModel();
	void handlePacket(ENetPacket* p);
	void sendUpdatedModel(AbstractModel* m);
	void sendRemoveModel(AbstractModel* m);
};
#endif
