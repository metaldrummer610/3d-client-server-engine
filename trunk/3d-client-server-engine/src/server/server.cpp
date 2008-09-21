#include <enet/enet.h>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#include "server.h"
#include "../util/common.h"

#define SERVER 1

int main(int argc, char ** argv) {
	Server s = Server();

	s.init();

	s.mainLoop();

	s.deinit();
	return 0;
}

void Server::init() {
	////////////////////
	// init enet
	////////////////////

	if (enet_initialize() != 0) {
		cout << "enet did not initialize" << endl;
		exit(EXIT_FAILURE);
	}

	address.host = ENET_HOST_ANY;
	address.port = 4445;

	server = enet_host_create(&address, 32, 0, 0);

	if (server == NULL) {
		cout << "server is null.... that is bad" << endl;
		exit(EXIT_FAILURE);
	}

	atexit(enet_deinitialize);

	peerNumber = 0;

	//////////////////////
	// sdl init - for the timer
	//////////////////////

	SDL_Init(SDL_INIT_TIMER);
}

void Server::deinit() {
	enet_host_destroy(server);
	delete &modelList;
}

void Server::addModelToList(ENetPeer* p) {
	AbstractModel* a = factory.getModelByName("resources/models/shape.obj");
	a->setX(0.0f);
	a->setY(0.2f);
	a->setZ(-10.3245f);
	a->setId((int&) p->data);
	modelList.insert(pair<int, AbstractModel*> (a->getId(), a));
}

void Server::sendModels(ENetPeer *p) {
	map<int, AbstractModel*>::iterator it;
	string name;

	for (it = modelList.begin(); it != modelList.end(); it++) {
		string s = (*it).second->serialize();

		stringstream ss(stringstream::in | stringstream::out);

		ss << "newModel," << s;

		ENetPacket* packet = enet_packet_create(ss.str().c_str(), strlen(
				ss.str().c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);

		enet_host_broadcast(server, 0, packet);
	}

	if (p != NULL) {
		int i = (int&) p->data;
		AbstractModel* a = modelList[i];
		stringstream ss(stringstream::in | stringstream::out);

		ss << "player," << a->getId() << "," << a->getName() << ",";

		ENetPacket* packet = enet_packet_create(ss.str().c_str(), strlen(
				ss.str().c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(p, 0, packet);
	}
}

void Server::loadChangedModels() {
	stringstream ss(stringstream::in | stringstream::out);

	std::map<int, AbstractModel*>::iterator it;

	ss << "reload,";

	for (it = modelList.begin(); it != modelList.end(); it++) {
		ss << (*it).second->serialize() << "!";
	}

	ENetPacket* packet = enet_packet_create(ss.str().c_str(), strlen(
			ss.str().c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);

	enet_host_broadcast(server, 0, packet);
}

void Server::handlePacket(ENetPacket* p) {

	stringstream ss(stringstream::in | stringstream::out);

	ss >> std::noskipws;

	ss << p->data;

	string s = ss.str();

	int i = s.find("move");

	if (i != -1) {
		i += 5;
		string temp = s.substr(i);

		string axis = "";
		float distance = 0;
		int id = 0;

		vector<string> args;

		splitString(temp, args, ",");

		istringstream in;
		in.str(args[0]);
		in >> axis;

		in.clear();
		in.str(args[1]);
		in >> distance;

		in.clear();
		in.str(args[2]);
		in >> id;

		AbstractModel* c = modelList.find(id)->second;
		// add in checks to make sure no collisions happen or the object goes off the screen
		if (axis == "x") {
			c->setX(c->getX() + distance);
		} else if (axis == "y") {
			c->setY(c->getY() + distance);
		} else if (axis == "z") {
			c->setZ(c->getZ() + distance);
		}

		sendUpdatedModel(c);

		return;
	}

	i = s.find("rotate");

	if (i != -1) {
		i += 7;
		string temp = s.substr(i);

		string axis = "";
		float distance = 0;
		int id = 0;

		vector<string> args;

		splitString(temp, args, ",");

		istringstream in;
		in.str(args[0]);
		in >> axis;

		in.clear();
		in.str(args[1]);
		in >> distance;

		in.clear();
		in.str(args[2]);
		in >> id;

		AbstractModel* c = modelList.find(id)->second;
		// add in checks to make sure no collisions happen or the object goes off the screen
		if (axis == "x") {
			c->setAngleX(c->getAngleX() + distance);
		} else if (axis == "y") {
			c->setAngleY(c->getAngleY() + distance);
		} else if (axis == "z") {
			c->setAngleZ(c->getAngleZ() + distance);
		}

		sendUpdatedModel(c);

		return;
	}

	i = s.find("changePlayer");

	if (i != -1) {
		i += 13;
		string masterStr = s.substr(i);

		int id = 0;
		string returnStr = "";

		vector<string> args;

		splitString(masterStr, args, ",");

		args.erase(args.begin() + 1);
		args.erase(args.end());

		vector<string>::iterator it;

		stringstream in;

		for (it = args.begin(); it != args.end(); it++) {
			in << *it << ",";
		}

		in >> returnStr;

		in.clear();
		in.str(args.back());
		in >> id;

		factory.updateModel(modelList.find(id)->second, returnStr);

		sendUpdatedModel(modelList.find(id)->second);

		return;
	}

	i = s.find("text");

	if (i != -1) {
		ENetPacket* packet = enet_packet_create(ss.str().c_str(), strlen(
				ss.str().c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);

		enet_host_broadcast(server, 0, packet);

		return;
	}
}

void Server::sendUpdatedModel(AbstractModel* m) {
	stringstream ss(stringstream::in | stringstream::out);

	ss << "updatedModel," << m->serialize();

	ENetPacket* packet = enet_packet_create(ss.str().c_str(), strlen(
			ss.str().c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);

	enet_host_broadcast(server, 0, packet);
}

void Server::sendRemoveModel(AbstractModel* m) {
	stringstream ss(stringstream::in | stringstream::out);

	ss << "removeModel," << m->getId();

	ENetPacket* packet = enet_packet_create(ss.str().c_str(), strlen(
			ss.str().c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);

	enet_host_broadcast(server, 0, packet);
}

void Server::mainLoop() {
	cout << "Hello World" << endl;

	ENetEvent event;

	int T0 = 0;

	while (true) {
		/* Wait up to 1000 milliseconds for an event. */
		while (enet_host_service(server, &event, 1) > 0) {
			switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT:
				/* Store any relevant client information here. */
				cout << "client connected" << endl;

				event.peer->data = (void*) peerNumber;
				addModelToList(event.peer);
				sendModels(event.peer);

				peerNumber++;
				break;

			case ENET_EVENT_TYPE_RECEIVE:
				//cout << "A packet of length " << event.packet->dataLength
				//		<< " containing " << event.packet->data
				//		<< " was received from " << event.peer->data
				//		<< " on channel " << event.channelID << "." << endl;
				/* Clean up the packet now that we're done using it. */
				handlePacket(event.packet);
				enet_packet_destroy(event.packet);
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				cout << event.peer -> data << " disconnected" << endl;
				sendRemoveModel(modelList[(int&) event.peer->data]);
				modelList.erase((int&) event.peer->data);

				/* Reset the peer's client information. */
				event.peer -> data = NULL;
			}
		}

		int t = SDL_GetTicks();
		if (t - T0 >= 10000) {
			T0 = t;
			loadChangedModels();
		}
	}
}

