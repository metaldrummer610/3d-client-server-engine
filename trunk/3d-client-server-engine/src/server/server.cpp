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

	peerNumber = 0;
}

void Server::deinit() {
	enet_host_destroy(server);
	delete &modelList;
}

void Server::addModelToList(ENetPeer* p) {
	AbstractModel* a = factory.getModelByName("resources/models/testBox.obj");
	a->setX(-9.0f);
	a->setY(5.2f);
	a->setZ(-23.3245f);
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

		return;
	}

	/*i = s.find("text");

	if (i != -1) {
		i += 5;
		string tmp = s.substr(i);

		string::iterator it;
		string temp = "";

		string text = "";
		int id = 0;

		bool textDone = false;
		bool idDone = false;

		for (it = tmp.begin(); it != tmp.end(); it++) {
			if (*it != ',') {
				char c = *it;
				//cout << "c is: " << *it << ":" << endl;
				temp.append(&c);
			} else {
				//cout << "temp is: " << temp << endl;
				istringstream in;
				in >> std::noskipws;
				in.str(temp);

				if (textDone == false) {
					text = in.str();
					textDone = true;
				} else if (idDone == false) {
					in >> id;
					idDone = true;
				}

				temp = "";
			}
		}

		stringstream ss(stringstream::in | stringstream::out);
		ss >> std::noskipws;

		ss << "text," << id << "," << text << ",";

		ENetPacket* packet = enet_packet_create(ss.str().c_str(), strlen(
				ss.str().c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);

		enet_host_broadcast(server, 0, packet);

		return;
	}*/

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
				cout << "A packet of length " << event.packet->dataLength
						<< " containing " << event.packet->data
						<< " was received from " << event.peer->data
						<< " on channel " << event.channelID << "." << endl;
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
	}
}

