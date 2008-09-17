#include "pyramid.h"
#include <iostream>
#include <sstream>
#include <string>
#include <SDL/SDL_opengl.h>

Pyramid::Pyramid() {
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	name = "pyramid";
	angle = 0.0f;
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

std::string Pyramid::serialize() {
	std::stringstream ss(std::stringstream::in | std::stringstream::out);

	ss << "pyramid" << "," << id << "," << x << "," << y << "," << z << "," << angleX << "," << angleY << "," << angleZ << ",";

	return ss.str();
}
