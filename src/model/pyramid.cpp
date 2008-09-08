#include "pyramid.h"
#include <iostream>
#include <sstream>
#include <SDL/SDL_opengl.h>
using namespace std;

static float angle = 0.0f;

Pyramid::Pyramid() {
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	name = "pyramid";
}

void Pyramid::draw() {
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
	glBegin(GL_TRIANGLES);

	glColor3f(1.0f, 0.0f, 0.0f); // Red
	glVertex3f(0.0f, 1.0f, 0.0f); // Top Of Triangle (Front)
	glColor3f(0.0f, 1.0f, 0.0f); // Green
	glVertex3f(-1.0f, -1.0f, 1.0f); // Left Of Triangle (Front)
	glColor3f(0.0f, 0.0f, 1.0f); // Blue
	glVertex3f(1.0f, -1.0f, 1.0f); // Right Of Triangle (Front)


	glColor3f(1.0f, 0.0f, 0.0f); // Red
	glVertex3f(0.0f, 1.0f, 0.0f); // Top Of Triangle (Right)
	glColor3f(0.0f, 0.0f, 1.0f); // Blue
	glVertex3f(1.0f, -1.0f, 1.0f); // Left Of Triangle (Right)
	glColor3f(0.0f, 1.0f, 0.0f); // Green
	glVertex3f(1.0f, -1.0f, -1.0f); // Right Of Triangle (Right)


	glColor3f(1.0f, 0.0f, 0.0f); // Red
	glVertex3f(0.0f, 1.0f, 0.0f); // Top Of Triangle (Back)
	glColor3f(0.0f, 1.0f, 0.0f); // Green
	glVertex3f(1.0f, -1.0f, -1.0f); // Left Of Triangle (Back)
	glColor3f(0.0f, 0.0f, 1.0f); // Blue
	glVertex3f(-1.0f, -1.0f, -1.0f); // Right Of Triangle (Back)


	glColor3f(1.0f, 0.0f, 0.0f); // Red
	glVertex3f(0.0f, 1.0f, 0.0f); // Top Of Triangle (Left)
	glColor3f(0.0f, 0.0f, 1.0f); // Blue
	glVertex3f(-1.0f, -1.0f, -1.0f); // Left Of Triangle (Left)
	glColor3f(0.0f, 1.0f, 0.0f); // Green
	glVertex3f(-1.0f, -1.0f, 1.0f); // Right Of Triangle (Left)

	glEnd();
}

string Pyramid::serialize() {
	stringstream ss(stringstream::in | stringstream::out);

	ss << "pyramid" << "," << id << "," << x << "," << y << "," << z << ",";

	return ss.str();
}

void Pyramid::deserialize(string& s) {
	string::iterator it;
	string temp;
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
			istringstream in(temp);

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
