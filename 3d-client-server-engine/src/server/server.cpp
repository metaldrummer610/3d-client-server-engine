#include <enet/enet.h>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#include "server.h"

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
}

void Server::deinit() {
	enet_host_destroy(server);
	delete &modelList;
}

AbstractModel* Server::createModel() {
	AbstractModel* a = factory.getModelByName("pyramid");
	a->setX(-9.0f);
	a->setY(5.2f);
	a->setZ(-23.3245f);
	a->setId(modelList.size());
	return a;
}

void Server::sendModels(ENetPeer *p) {
	/*	map<int, ENetPeer*>::iterator piter;
	 map<int, AbstractModel*>::iterator it;
	 string name;

	 for (piter = peerList.begin(); piter != peerList.end(); piter++) {
	 for (it = modelList.begin(); it != modelList.end(); it++) {
	 string s = (*it).second->serialize();

	 stringstream ss(stringstream::in | stringstream::out);

	 ss << "newModel," << s;

	 ENetPacket* packet = enet_packet_create(ss.str().c_str(), strlen(
	 ss.str().c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);
	 enet_peer_send((*piter).second, 0, packet);
	 }
	 }

	 if (p != NULL) {
	 stringstream ss(stringstream::in | stringstream::out);
	 ss << "player," << modelList.size() - 1 << "," << modelList.find(
	 modelList.size() - 1)->second->getName() << ",";

	 ENetPacket* packet = enet_packet_create(ss.str().c_str(), strlen(
	 ss.str().c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);
	 enet_peer_send(p, 0, packet);
	 }
	 */
	ENetPeer* peers[] = server->peers;
	cout << "peer data " << server->peerCount << endl;

	for (int i = 0; i < peers->length; i++) {
		cout << "inside loop" << endl;
		AbstractModel* a = (AbstractModel*) peers[i].data;

		string s = a->serialize();

		cout << "created" << endl;
		stringstream ss(stringstream::in | stringstream::out);

		ss << "newModel," << s;

		ENetPacket* packet = enet_packet_create(ss.str().c_str(), strlen(
				ss.str().c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(&peers[i], 0, packet);
	}

}

void Server::handlePacket(ENetPacket* p) {

	stringstream ss(stringstream::in | stringstream::out);

	ss << p->data;

	string s = ss.str();

	int i = s.find("move");

	if (i != -1) {
		i += 5;
		string tmp = s.substr(i);

		string::iterator it;
		string temp;
		temp = "";

		string axis = "";
		float distance = 0;
		int id = 0;

		bool axisDone = false;
		bool distanceDone = false;
		bool idDone = false;

		for (it = tmp.begin(); it != tmp.end(); it++) {
			if (*it != ',') {
				char c = *it;
				//cout << "c is: " << *it << ":" << endl;
				temp.append(&c);
			} else {
				//cout << "temp is: " << temp << endl;
				istringstream in(temp);

				if (axisDone == false) {
					in >> axis;
					axisDone = true;
				} else if (distanceDone == false) {
					in >> distance;
					distanceDone = true;
				} else if (idDone == false) {
					in >> id;
					idDone = true;
				}

				temp = "";
			}
		}

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
	}
}

void Server::sendUpdatedModel(AbstractModel* m) {
	stringstream ss(stringstream::in | stringstream::out);

	ss << "updatedModel," << m->serialize();

	ENetPacket* packet = enet_packet_create(ss.str().c_str(), strlen(
			ss.str().c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);

	enet_host_broadcast(server, 0, packet);

	/*map<int, ENetPeer*>::iterator piter;

	 for (piter = peerList.begin(); piter != peerList.end(); piter++) {
	 stringstream ss(stringstream::in | stringstream::out);

	 ss << "updatedModel," << m->serialize();

	 ENetPacket* packet = enet_packet_create(ss.str().c_str(), strlen(
	 ss.str().c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);
	 enet_peer_send((*piter).second, 0, packet);
	 }*/
}

void Server::mainLoop() {
	cout << "Hello World" << endl;

	ENetEvent event;

	while (true) {
		/* Wait up to 1000 milliseconds for an event. */
		while (enet_host_service(server, &event, 1) > 0) {
			switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT:
				/* Store any relevant client information here. */
				cout << "client connected" << endl;
				//peerList.insert(pair<int, ENetPeer*> (peerList.size() + 1,
				//event.peer));

				event.peer->data = createModel();
				sendModels(event.peer);

				break;

			case ENET_EVENT_TYPE_RECEIVE:
				printf(
						"A packet of length %u containing %s was received from %s on channel %u.\n",
						event.packet -> dataLength, event.packet -> data,
						event.peer -> data, event.channelID);
				/* Clean up the packet now that we're done using it. */
				handlePacket(event.packet);
				enet_packet_destroy(event.packet);
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				/* Reset the peer's client information. */
				event.peer -> data = NULL;

				/*map<int, ENetPeer*>::iterator it;
				 map<int, AbstractModel*>::iterator it2;

				 for (it = peerList.begin(); it != peerList.end(); it++) {
				 if ((*it).second == event.peer) {
				 it2 = modelList.find((*it).first);
				 modelList.erase(it2);
				 peerList.erase(it);
				 cout << "client and model erased" << endl;
				 }
				 }*/

				cout << "client disconnected" << endl;
			}
		}
	}
}

