#include "cube.h"
#include <iostream>
#include <sstream>
#include <string>
#include <SDL/SDL_opengl.h>

static float angle = 0.0f;

Cube::Cube() {
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	name = "cube";
}

void Cube::draw() {
	/* Our angle of rotation. */

	glLoadIdentity();

	/* Move down the z-axis. */
	glTranslatef(x, y, z);

	/* Rotate. */
	glRotatef(angle, 0.0, 1.0, 0.0);

	if (++angle > 360.0f) {
		angle = 0.0f;
	}

	/* Send our triangle data to the pipeline. */
	glBegin(GL_QUADS); // Draw A Quad

	glColor3f(0.0f, 1.0f, 0.0f); // Set The Color To Green
	glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Quad (Top)
	glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Quad (Top)
	glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Quad (Top)
	glVertex3f(1.0f, 1.0f, 1.0f); // Bottom Right Of The Quad (Top)

	glColor3f(1.0f, 0.5f, 0.0f); // Set The Color To Orange
	glVertex3f(1.0f, -1.0f, 1.0f); // Top Right Of The Quad (Bottom)
	glVertex3f(-1.0f, -1.0f, 1.0f); // Top Left Of The Quad (Bottom)
	glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Quad (Bottom)
	glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Right Of The Quad (Bottom)

	glColor3f(1.0f, 0.0f, 0.0f); // Set The Color To Red
	glVertex3f(1.0f, 1.0f, 1.0f); // Top Right Of The Quad (Front)
	glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Quad (Front)
	glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Quad (Front)
	glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Right Of The Quad (Front)

	glColor3f(1.0f, 1.0f, 0.0f); // Set The Color To Yellow
	glVertex3f(1.0f, -1.0f, -1.0f); // Top Right Of The Quad (Back)
	glVertex3f(-1.0f, -1.0f, -1.0f); // Top Left Of The Quad (Back)
	glVertex3f(-1.0f, 1.0f, -1.0f); // Bottom Left Of The Quad (Back)
	glVertex3f(1.0f, 1.0f, -1.0f); // Bottom Right Of The Quad (Back)

	glColor3f(0.0f, 0.0f, 1.0f); // Set The Color To Blue
	glVertex3f(-1.0f, 1.0f, 1.0f); // Top Right Of The Quad (Left)
	glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Quad (Left)
	glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Quad (Left)
	glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Quad (Left)

	glColor3f(1.0f, 0.0f, 1.0f); // Set The Color To Violet
	glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Quad (Right)
	glVertex3f(1.0f, 1.0f, 1.0f); // Top Left Of The Quad (Right)
	glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Quad (Right)
	glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Right Of The Quad (Right)

	glEnd();
}

std::string Cube::serialize() {
	std::stringstream ss(std::stringstream::in | std::stringstream::out);

	ss << "cube" << "," << id << "," << x << "," << y << "," << z << ",";

	return ss.str();
}

void Cube::deserialize(std::string& s) {
	std::string::iterator it;
	std::string temp;
	temp = "";

	bool idDone = false;
	bool xDone = false;
	bool yDone = false;
	bool zDone = false;

	for (it = s.begin(); it != s.end(); it++) {
		if (*it != ',') {
			char c = *it;
			//cout << "c is: " << *it << ":" << endl;
			temp.append(&c);
		} else {
			//cout << "temp is: " << temp << endl;
			std::istringstream in(temp);

			if (idDone == false) {
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
			}

			temp = "";
		}
	}
}
