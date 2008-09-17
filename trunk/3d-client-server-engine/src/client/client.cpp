#include <enet/enet.h>
#include <iostream>
#include <sstream>
using namespace std;

#include "client.h"
#include "../model/modelFactory.h"
#include "../util/common.h"

int main(int argv, char** argc) {
	Client c = Client();
	c.init();

	c.mainLoop();

	c.deinit();
	return 0;
}

void Client::init() {
	T0 = 0;
	Frames = 0;
	seconds = 0;
	fps = 0;
	renderFPS = false;

	/////////////////////
	// load up config file and set properties
	/////////////////////

	properties = fileLoader.loadConfigFile();

	std::istringstream width(properties.find("SCREEN_WIDTH")->second);
	width >> SCREEN_WIDTH;

	std::istringstream height(properties.find("SCREEN_HEIGHT")->second);
	height >> SCREEN_HEIGHT;

	////////////////////
	// this is the enet init
	////////////////////
	if (enet_initialize() != 0) {
		cout << "enet failed to initialize" << endl;
		exit(EXIT_FAILURE);
	}

	client = enet_host_create(NULL, 1, 0, 0);

	if (client == NULL) {
		cout << "client is null" << endl;
		exit(EXIT_FAILURE);
	}

	enet_address_set_host(&address,
			properties.find("SERVER_ADDRESS")->second.c_str());
	address.port = 4445;

	peer = enet_host_connect(client, &address, 2);

	if (peer == NULL) {
		cout << "no peer was created" << endl;
		exit(EXIT_FAILURE);
	}

	////////////////////
	// this is the SDL and OpenGL init
	////////////////////
	sdl_openglInit(SCREEN_WIDTH, SCREEN_HEIGHT);
}

void Client::sdl_openglInit(int width, int height) {

	/* Information about the current video settings. */
	const SDL_VideoInfo* info = NULL;
	/* Color depth in bits of our window. */
	bpp = 0;
	/* Flags we will pass into SDL_SetVideoMode. */
	int flags = 0;

	/* First, initialize SDL's video subsystem. */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		/* Failed, exit. */
		fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
		exit(1);
	}

	/* Let's get some video information. */
	info = SDL_GetVideoInfo();

	if (!info) {
		/* This should probably never happen. */
		fprintf(stderr, "Video query failed: %s\n", SDL_GetError());
		exit(1);
	}

	bpp = info->vfmt->BitsPerPixel;

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	flags = SDL_OPENGL | SDL_RESIZABLE;

	/*
	 * Set the video modestring fpsStr;
	 */
	if (SDL_SetVideoMode(width, height, bpp, flags) == 0) {
		/*
		 * This could happen for a variety of reasons,
		 * including DISPLAY not being set, the specified
		 * resolution not being available, etc.
		 */
		fprintf(stderr, "Video mode set failed: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	/////////////////////////////
	// opengl setup
	/////////////////////////////

	float ratio = (float) width / (float) height;

	// Somewhere in the initialization part of your program…
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Create light components
	GLfloat ambientLight[] = { 0.7f, 0.2f, 0.7f, 1.0f };
	GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
	GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat position[] = { -1.5f, 1.0f, -4.0f, 1.0f };

	// Assign created components to GL_LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	/* Our shading model--Gouraud (smooth). */
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);

	/* Culling. */
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	/* Set the clear color. */
	glClearColor(0, 0, 0, 1.0f);

	/* Setup our viewport. */
	glViewport(0, 0, width, height);

	/*
	 * Change to the projection matrix and set
	 * our viewing volume.
	 */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0, ratio, 1.0, 1024.0);
	resizeWindow(width, height);
}

void Client::deinit() {
	enet_peer_disconnect(peer, 0);

	while (enet_host_service(client, &event, 3000) > 0) {
		switch (event.type) {
		case ENET_EVENT_TYPE_DISCONNECT:
			printf("%s disconnected.\n", event.peer -> data);

			/* Reset the peer's client information. */
			event.peer -> data = NULL;
		}
	}

	enet_host_destroy(client);
}

void Client::addEventToStack(std::string x, ...) {
	char text[256]; /* Holds our string */
	va_list ap; /* Pointer to our list of elements */

	const char* tmp = x.c_str();

	/* Parses The String For Variables */
	va_start(ap, tmp);
	/* Converts Symbols To Actual Numbers */
	vsprintf(text, tmp, ap);
	va_end(ap);

	std::string s(text);

	events.addEvent(s, SDL_GetTicks());
}

void Client::render() {
	/* Clear the color and depth buffers. */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* We don't want to modify the projection matrix. */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	getFPS();
	displayEvents();

	glLoadIdentity();

	map<int, AbstractModel*>::iterator it;

	for (it = modelList.begin(); it != modelList.end(); it++) {
		(*it).second->draw();
	}

	glFlush();

	SDL_GL_SwapBuffers();

	/* Increment our FPS counter */
	Frames++;
}

void Client::displayEvents() {
	int ticks = SDL_GetTicks();

	if (!events.isEmpty()) {
		while (ticks - events.getTimeOnStack() >= 5000) {
			if (!events.deleteEvent())
				return;
		}

		if (!events.isEmpty()) {
			std::deque<std::string>::const_iterator it;

			int currentY = fontFactory.getFontSize() + 5;
			for (it = events.getFront(); it != events.getBack(); it++) {
				currentY = currentY + fontFactory.getFontSize() + 5;
				fontFactory.glPrint(0, currentY, &(*it->c_str()));
			}
		}
	}

}

void Client::getFPS() {
	if (renderFPS) {
		fontFactory.glPrint(0, SCREEN_HEIGHT - 35,
				"%d frames in %g seconds = %g FPS", totalFrames, seconds, fps);
	}
	GLint t = SDL_GetTicks();
	if (t - T0 >= 1000) {
		seconds = (t - T0) / 1000.0;
		fps = Frames / seconds;
		totalFrames = Frames;
		T0 = t;
		Frames = 0;
	}
}

void Client::handlePacket(ENetPacket *p) {
	stringstream ss(stringstream::in | stringstream::out);

	ss >> std::noskipws;

	ss << p->data;

	string s = ss.str();

	int i = s.find("newModel");

	if (i != -1) {
		i += 9;
		string tmp = s.substr(i);

		AbstractModel* a = modelFactory.getModel(tmp);

		modelList.insert(pair<int, AbstractModel*> (a->getId(), a));

		addEventToStack("A player has joined the game");
		return;
	}

	i = s.find("updatedModel");

	if (i != -1) {
		i += 13;
		string str = s.substr(i);

		AbstractModel* c = modelFactory.getModel(str);

		AbstractModel* a = modelList[c->getId()];

		a->setX(c->getX());
		a->setY(c->getY());
		a->setZ(c->getZ());
		a->setAngleX(c->getAngleX());
		a->setAngleY(c->getAngleY());
		a->setAngleZ(c->getAngleZ());

		return;
	}

	i = s.find("removeModel");

	if (i != -1) {
		i += 12;
		string str = s.substr(i);

		istringstream ss(str);

		int id = 0;

		ss >> id;

		modelList.erase(id);

		addEventToStack("A player has left the game");

		return;
	}

	i = s.find("player");

	if (i != -1) {
		i += 7;

		string str = s.substr(i);
		int spot;
		string name;

		vector<string> args;

		splitString(str, args, ",");

		istringstream in;
		in.str(args[0]);
		in >> spot;

		in.clear();
		in.str(args[1]);
		in >> name;

		AbstractModel* c = modelFactory.getModelByName(name);
		c = modelList[spot];
		player = c;

		return;
	}

	i = s.find("text");

	if (i != -1) {
		i += 5;
		string tmp = s.substr(i);

		string text = "";
		int id = 0;

		vector<string> args;

		splitString(tmp, args, ",");

		istringstream in;
		in >> std::noskipws;
		in.str(args[0]);
		in >> text;

		cout << in.str() << endl;

		in.clear();
		in.str(args[1]);
		in >> id;

		cout << text << endl;

		addEventToStack("[%d] %s", id, text.c_str());
		return;
	}

	/*i = s.find("reload");

	if (i != -1) {
		i += 7;
		std::string::iterator it;
		string tmp = s.substr(i);
		std::cout << temp << endl
		std::string temp;
		temp = "";

		std::string name;
		int id;
		float x;
		float y;
		float z;

		bool nameDone = false;
		bool idDone = false;
		bool xDone = false;
		bool yDone = false;
		bool zDone = false;



		for (it = tmp.begin(); it != tmp.end(); it++) {
			if (*it != ',') {
				char c = *it;
				//cout << "c is: " << *it << ":" << endl;
				temp.append(&c);
			} else {
				//cout << "temp is: " << temp << endl;
				std::istringstream in;
				in.str(temp);

				if (nameDone == false) {
					in >> name;
					nameDone = true;
				} else if (idDone == false) {
					in >> id;
					idDone = true;
				} else if (xDone == false) {
					in >> x;
					xDone = true;
				} else if (yDone == false) {
					in >> y;
					yDone = true;
				} else if (zDone == false) {
					in >> z;
					zDone = true;
				} else if(nameDone == true && idDone == true && xDone == true && yDone == true && zDone == true){
					map<int, AbstractModel*>::iterator it;

					std::cout << "in last if" << std::endl;

					for (it = modelList.begin(); it != modelList.end(); it++) {
						if((*it).second->getName().compare(name) == 0){
							AbstractModel* c = modelFactory.getModelByName(temp);
							(*it).second = c;
							(*it).second->setId(c->getId());
							(*it).second->setX(c->getX());
							(*it).second->setY(c->getY());
							(*it).second->setZ(c->getZ());

							std::cout << "id is " << (*it).first << std::endl;
							if((*it).first == player->getId()){
								std::cout << "player id is found" << std::endl;
								player = (*it).second;
							}
						}
					}

					name = "";
					id = 0;
					x = 0;
					y = 0;
					z = 0;

					nameDone = false;
					idDone = false;
					xDone = false;
					yDone = false;
					zDone = false;
				}

				temp = "";
			}
		}

		map<int, AbstractModel*>::iterator it2;

		std::cout << "out of loop" << std::endl;

		for (it2 = modelList.begin(); it2 != modelList.end(); it2++) {
			std::cout << "name is " << name << std::endl;
			if((*it2).second->getName().compare(name) == 0){
				AbstractModel* c = modelFactory.getModelByName(temp);
				(*it2).second = c;
				(*it2).second->setId(c->getId());
				(*it2).second->setX(c->getX());
				(*it2).second->setY(c->getY());
				(*it2).second->setZ(c->getZ());

				std::cout << "id is " << (*it2).first << std::endl;
				if((*it2).first == player->getId()){
					std::cout << "player id is found" << std::endl;
					player = (*it2).second;
				}
			}
		}}*/
}

/* function to reset our viewport after a window resize */
int Client::resizeWindow(int width, int height) {
	/* Height / width ration */
	GLfloat ratio;

	/* Protect against a divide by zero */
	if (height == 0)
		height = 1;

	ratio = (GLfloat) width / (GLfloat) height;

	/* Setup our viewport. */
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);

	/* change to the projection matrix and set our viewing volume. */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* Set our perspective */
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	/* Make sure we're chaning the model view and not the projection */
	glMatrixMode(GL_MODELVIEW);

	/* Reset The View */
	glLoadIdentity();
}

void Client::sendPacket(string s) {
	stringstream ss(stringstream::in | stringstream::out);
	ss << s << "," << player->getId() << ",";

	string str = ss.str();

	ENetPacket* packet = enet_packet_create(str.c_str(), strlen(str.c_str())
			+ 1, ENET_PACKET_FLAG_RELIABLE);

	enet_peer_send(peer, 0, packet);
}

/* function to handle key press events */
void Client::handleKeyPress(SDL_keysym *keysym) {
	switch (keysym->sym) {
	case SDLK_ESCAPE:
		/* ESC key was pressed */
		deinit();
		exit(EXIT_FAILURE);
		break;
		// taken out because the full screen doesn't work
		//case SDLK_F1:
		/* F1 key was pressed
		 * this toggles fullscreen mode
		 */
		//	SDL_WM_ToggleFullScreen(screen);
		//	break;
	case SDLK_TAB:
		if (!renderFPS)
			renderFPS = true;
		else
			renderFPS = false;
		break;
	case SDLK_1:
		sendPacket("text,Hello World");
		break;

	case SDLK_w:
		sendPacket("move,y,0.125");
		player->setY(player->getY() + 0.125f);
		break;
	case SDLK_a:
		sendPacket("move,x,-0.125");
		player->setX(player->getX() - 0.125f);
		break;
	case SDLK_s:
		sendPacket("move,y,-0.125");
		player->setY(player->getY() - 0.125f);
		break;
	case SDLK_d:
		sendPacket("move,x,0.125");
		player->setX(player->getX() + 0.125f);
		break;
	case SDLK_q:
		sendPacket("move,z,-0.125");
		player->setZ(player->getZ() - 0.125f);
		break;
	case SDLK_e:
		sendPacket("move,z,0.125");
		player->setZ(player->getZ() + 0.125f);
		break;

		// rotates the model
	case SDLK_u:
		sendPacket("rotate,y,0.125");
		player->setAngleY(player->getAngleY() + 0.125f);
		break;
	case SDLK_h:
		sendPacket("rotate,x,-0.125");
		player->setAngleX(player->getAngleX() - 0.125f);
		break;
	case SDLK_j:
		sendPacket("rotate,y,-0.125");
		player->setAngleY(player->getAngleY() - 0.125f);
		break;
	case SDLK_k:
		sendPacket("rotate,x,0.125");
		player->setAngleX(player->getAngleX() + 0.125f);
		break;
	case SDLK_y:
		sendPacket("rotate,z,-0.125");
		player->setAngleZ(player->getAngleZ() - 0.125f);
		break;
	case SDLK_i:
		sendPacket("rotate,z,0.125");
		player->setAngleZ(player->getAngleZ() + 0.125f);
		break;

	case SDLK_UP:
		fontFactory.setFontSize(fontFactory.getFontSize() + 1);
		break;
	case SDLK_DOWN:
		fontFactory.setFontSize(fontFactory.getFontSize() - 1);
		break;
	default:
		break;
	}

	return;
}

void Client::processSdlEvents() {
	SDL_Event sdlEvent;

	while (SDL_PollEvent(&sdlEvent)) {
		switch (sdlEvent.type) {
		/*case SDL_ACTIVEEVENT:
		 Something's happend with our focus
		 * If we lost focus or we are iconified, we
		 * shouldn't draw the screen

		 if (sdlEvent.active.gain == 0) {
		 isActive = false;
		 } else {
		 isActive = true;
		 }
		 break;*/
		case SDL_KEYDOWN:
			/* handle key presses */
			handleKeyPress(&sdlEvent.key.keysym);
			break;
		case SDL_QUIT:
			exit(EXIT_FAILURE);
		default:
			break;
		}
	}

	/* draw the scene */
	//if (isActive)
	render();

}

void Client::mainLoop() {
	cout << "Hello World!!!" << endl;

	///////////////////
	// enet main loop for events
	///////////////////
	while (true) {
		while (enet_host_service(client, &event, 1) > 0) {
			switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT:
				//addEventToStack("A new client connected from %x:%u",
				//		event.peer -> address.host, event.peer -> address.port);

				/* Store any relevant client information here. */
				event.peer -> data = (void*) "Client information";

				break;

			case ENET_EVENT_TYPE_RECEIVE:
				handlePacket(event.packet);
				cout << "event data " << event.packet->data << endl;

				/* Clean up the packet now that we're done using it. */
				enet_packet_destroy(event.packet);
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				printf("%s disconected.\n", event.peer -> data);

				/* Reset the peer's client information. */
				event.peer -> data = NULL;
			}
		}

		///////////////////
		// sdl main loop for events
		///////////////////

		processSdlEvents();
	}
}
