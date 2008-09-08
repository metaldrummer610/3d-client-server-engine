#ifndef _SERVER_H
#define _SERVER_H
#include <enet/enet.h>
#include <map>
#include "../model/abstractModel.h"
#include "../model/modelFactory.h"
class Server {

private:
	ENetHost* server;
	ENetAddress address;
	std::map<int, AbstractModel*> modelList;
	//std::map<int, ENetPeer*> peerList;
	ModelFactory factory;
	int peerNumber;

public:
	void init();
	void deinit();
	void mainLoop();
	void sendModels(ENetPeer *p);
	void addModelToList(ENetPeer *p);
	AbstractModel* createModel();
	void handlePacket(ENetPacket* p);
	void sendUpdatedModel(AbstractModel* m);
};
#endif
