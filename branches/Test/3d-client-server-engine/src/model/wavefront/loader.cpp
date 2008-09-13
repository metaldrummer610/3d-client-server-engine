/*
 * loader.cpp
 *
 *  Created on: Sep 13, 2008
 *      Author: metal
 */

#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include "loader.h"

AbstractModel* Loader::loadFile(std::string &s) {
	WaveFrontModel* m = new WaveFrontModel(s);

	std::ifstream file;

	file.open(s.c_str());

	std::stringstream ss(std::stringstream::in | std::stringstream::out);
	std::string line;
	std::string value;

	if (file.is_open()) {
		//std::cout << "file is open" << std::endl;
		while (!file.eof()) {
			getline(file, line);
			ss.clear();
			ss.str(line);
			value = "";
			ss >> value;

			//std::cout << "line is " << value << std::endl;
			//std::cout << "ss is " << ss.str() << std::endl;

			if (value.compare("mtllib") == 0) {
				//std::cout << "mtllib not supported yet" << std::endl;
			} else if (value.compare("o") == 0) {
				//std::cout << "o not supported yet" << std::endl;
			} else if (value.compare("#") == 0) {
				//std::cout << "# not supported yet" << std::endl;
			} else if (value.compare("usemtl") == 0) {
				//std::cout << "usemtl not supported yet" << std::endl;
			} else if (value.compare("s") == 0) {
				//std::cout << "s not supported yet" << std::endl;
			} else if (value.compare("v") == 0) {
				//std::cout << "adding vertex" << std::endl;
				std::string tmp = ss.str().substr(2);
				addVertex(tmp, m);
			} else if (value.compare("f") == 0) {
				//std::cout << "adding face" << std::endl;
				std::string tmp = ss.str().substr(2);
				addFace(tmp, m);
			}
		}
	}

	/*std::vector<Face>::iterator faceIt;
	 std::vector<Vertex>::iterator vertexIt;
	 std::vector<Point>::iterator pointIt;

	 int x, y, z = 0;

	 for (faceIt = m->faces.begin(); faceIt != m->faces.end(); faceIt++) {
	 for (pointIt = (*faceIt).points.begin(); pointIt
	 != (*faceIt).points.end(); pointIt++) {
	 std::cout << "point " << y << " in face " << x << " "
	 << (*pointIt).vertex << std::endl;
	 y++;
	 }
	 x++;
	 }

	 for (vertexIt = m->verticies.begin(); vertexIt != m->verticies.end(); vertexIt++) {
	 std::cout << "vertex x " << z << " " << (*vertexIt).x << std::endl;
	 std::cout << "vertex y " << z << " " << (*vertexIt).y << std::endl;
	 std::cout << "vertex z " << z << " " << (*vertexIt).z << std::endl;
	 z++;
	 }*/

	AbstractModel* a = m;
	//std::cout << "returning" << std::endl;
	return a;
}

void Loader::addFace(std::string s, WaveFrontModel* m) {
	// parse the string
	// send the parsed string values to the model

	std::vector<int> points;

	std::istringstream ss;
	ss.str(s);

	while (!ss.eof()) {
		int i = 0;
		ss >> i;
		points.push_back(i);
	}

	m->addFace(points);
}

void Loader::addVertex(std::string s, WaveFrontModel *m) {
	// v 0.592394 -1.000000 -0.909421

	std::vector<float> verticies;

	std::istringstream ss;
	ss.str(s);

	while (!ss.eof()) {
		float i = 0;
		ss >> i;
		verticies.push_back(i);
	}

	m->addVertex(verticies.at(0), verticies.at(1), verticies.at(2));
}
