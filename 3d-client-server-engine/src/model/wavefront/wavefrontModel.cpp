/*
 * wavefrontModel.h
 *
 *  Created on: Sep 16, 2008
 *      Author: diazr
 */

#include <SDL/SDL_opengl.h>
#include <sstream>
#include "wavefrontModel.h"

void WavefrontModel::addVertex(float x, float y, float z) {
	Vertex v;
	v.x = x;
	v.y = y;
	v.z = z;

	vertices.push_back(v);
}

void WavefrontModel::addNormal(float i, float j, float k) {
	Normal n;
	n.i = i;
	n.j = j;
	n.k = k;

	normals.push_back(n);
}

void WavefrontModel::addTexture(float x, float y, float z) {
	Texture t;
	t.x = x;
	t.y = y;
	t.z = z;

	textures.push_back(t);
}

void WavefrontModel::addFace(vector<int> vertices, vector<int> textures,
		vector<int> normals) {
	Face f;

	for (int i = 0; i < vertices.size(); i++) {
		Point p;

		p.vertex = vertices.at(i);
		p.texture = textures.at(i);
		p.normal = normals.at(i);

		f.points.push_back(p);
	}

	faces.push_back(f);
}

void WavefrontModel::draw() {
	glLoadIdentity();

	glTranslatef(x, y, z);


	vector<Face>::iterator faceit;
	vector<Point>::iterator pointit;

	for (faceit = faces.begin(); faceit != faces.end(); faceit++) {
		if ((*faceit).points.size() == 4)
			glBegin(GL_QUADS);
		else
			glBegin(GL_TRIANGLE_STRIP);
		for (pointit = (*faceit).points.begin(); pointit
				!= (*faceit).points.end(); pointit++) {
			if ((*pointit).normal != 0) {
				glNormal3f(normals[(*pointit).normal].i,
						normals[(*pointit).normal].j,
						normals[(*pointit).normal].k);
			}
			glVertex3f(vertices[(*pointit).vertex].x,
					vertices[(*pointit).vertex].y, vertices[(*pointit).vertex].z);
		}
		glEnd();
	}
}

std::string WavefrontModel::serialize() {
	std::stringstream ss(std::stringstream::in | std::stringstream::out);

	ss << name << "," << id << "," << x << "," << y << "," << z << ",";

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
