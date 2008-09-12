#include <iostream>
#include <algorithm>
#include <iomanip>

#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>

using namespace std;

#include <GL/gl.h>
#include "model.h"

struct WavefrontRenderer {
	void setup() {
		drawVertex = boost::bind(&WavefrontModel::drawVertex, boost::ref(
				*parent), _1);
		drawFace = boost::bind(&WavefrontModel::drawFace, boost::ref(*parent),
				_1);
	}
	WavefrontModel* parent;
	boost::function<void (Face&)> drawFace;
	boost::function<void (Point&)> drawVertex;
};

WavefrontModel::WavefrontModel() :
	vertexCount(0), faceCount(0), normalCount(0), renderer(
			new WavefrontRenderer) {
	renderer->parent = this;
	renderer->setup();
}

float x = 0.0f;

void WavefrontModel::drawVertex(Point& p) {
	if (p.normal != 0) {
		glNormal3f(normals[p.normal - 1].i, normals[p.normal - 1].j,
				normals[p.normal - 1].k);
	}
	glVertex3f(vertices[p.vertex - 1].x, vertices[p.vertex - 1].y,
			vertices[p.vertex - 1].z);
}

void WavefrontModel::drawFace(Face& f) {
	if (f.points.size() == 4)
		glBegin(GL_QUADS);
	else
		glBegin(GL_TRIANGLE_STRIP);
	for_each(f.points.begin(), f.points.end(), renderer->drawVertex);
	glEnd();
}

void WavefrontModel::draw() {
	//render each face
	for_each(faces.begin(), faces.end(), renderer->drawFace);
}

std::string WavefrontModel::serialize() {
	std::stringstream ss(std::stringstream::in | std::stringstream::out);

	ss << name << "," << id << "," << x << "," << y << "," << z << ",";

	cout << "serialized string is " << ss.str() << endl;
	return ss.str();
}

void WavefrontModel::deserialize(std::string& s) {
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
			cout << "c is: " << *it << ":" << endl;
			temp.append(&c);
		} else {
			cout << "temp is: " << temp << endl;
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
