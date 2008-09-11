#include <enet/enet.h>
#include <iostream>
#include <sstream>
using namespace std;

#include "client.h"
#include "../model/modelFactory.h"

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
		string fpsStr;
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
	glClearColor(0, 0, 1.0f, 1.0f);

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
		while (ticks - events.getTimeOnStack() >= 2500) {
			if(!events.deleteEvent())
				return;
		}

		if (!events.isEmpty()) {
			std::deque<std::string>::const_iterator it;

			int currentY = fontFactory.getFontSize() + 5;

			for (it = events.getFront(); it != events.getBack(); it++) {
				fontFactory.glPrint(0,
						currentY + fontFactory.getFontSize() + 5,
						&(*it->c_str()));
			}
		}
	}
}

void Client::getFPS() {
	if (renderFPS) {
		fontFactory.glPrint(0, SCREEN_HEIGHT - 35,
				"%d frames in %g seconds = %g FPS\n", totalFrames, seconds, fps);
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

		modelList[c->getId()] = c;

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
		string::iterator it;
		string temp;
		int spot;
		string name;

		bool spotDone = false;
		bool nameDone = false;

		string str = s.substr(i);

		for (it = str.begin(); it != str.end(); it++) {
			if (*it != ',') {
				char c = *it;
				//cout << "c is: " << *it << ":" << endl;
				temp.append(&c);
			} else {
				//cout << "temp is: " << temp << endl;
				istringstream in(temp);

				if (spotDone == false) {
					in >> spot;
					spotDone = true;
				} else if (nameDone == false) {
					in >> name;
					nameDone = true;
				}

				temp = "";
			}
		}

		AbstractModel* c = modelFactory.getModelByName(name);
		c = modelList[spot];
		player = c;

		return;
	}
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
				//cout << "event data " << event.packet->data << endl;

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
