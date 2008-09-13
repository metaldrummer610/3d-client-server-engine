/*
 * wavefrontModel.cpp
 *
 *  Created on: Sep 13, 2008
 *      Author: metal
 */

#include <sstream>
#include <iostream>
#include "wavefrontModel.h"

WaveFrontModel::WaveFrontModel(std::string &s) {
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	name = s;
	//angle = 0.0f;
}

void WaveFrontModel::addFace(std::vector<int> vertexPos) {
	Point p;
	Face f;

	std::vector<int>::iterator it;

	for (it = vertexPos.begin(); it != vertexPos.end(); it++) {
		p.vertex = *it;
		f.points.push_back(p);
	}

	faces.push_back(f);
}

void WaveFrontModel::addVertex(float x, float y, float z) {
	Vertex v;
	v.x = x;
	v.y = y;
	v.z = z;

	verticies.push_back(v);
}

void WaveFrontModel::draw() {
	/* Our angle of rotation. */

	glLoadIdentity();

	/* Move down the z-axis. */
	glTranslatef(x, y, z);

	std::vector<Face>::iterator faceIt;
	std::vector<Point>::iterator pointIt;

	glBegin(GL_QUADS);
	for (faceIt = faces.begin(); faceIt != faces.end(); faceIt++) {
		for (pointIt = (*faceIt).points.begin(); pointIt != (*faceIt).points.end(); pointIt++) {
			glVertex3f(
					verticies.at((*pointIt).vertex - 1).x,
					verticies.at((*pointIt).vertex - 1).y,
					verticies.at((*pointIt).vertex - 1).z
					);
		}
	}
	glEnd();

}

std::string WaveFrontModel::serialize() {
	std::stringstream ss(std::stringstream::in | std::stringstream::out);

	ss << name << "," << id << "," << x << "," << y << "," << z << ",";

	return ss.str();
}

void WaveFrontModel::deserialize(std::string& s) {
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
