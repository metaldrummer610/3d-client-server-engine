/*
 * wavefrontLoader.cpp
 *
 *  Created on: Sep 16, 2008
 *      Author: diazr
 */

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include "wavefrontLoader.h"
#include "../../util/common.h"

AbstractModel* WavefrontLoader::load(const char* str) {
	WavefrontModel* m = new WavefrontModel();

	m->setName(str);
#ifndef SERVER
	std::ifstream file;

	file.open(str);

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
				handleVertex(m, tmp);
			} else if (value.compare("f") == 0) {
				//std::cout << "adding face" << std::endl;
				std::string tmp = ss.str().substr(2);
				handleFace(m, tmp);
			} else if (value.compare("vn") == 0) {
				//std::cout << "adding face" << std::endl;
				std::string tmp = ss.str().substr(2);
				handleNormal(m, tmp);
			} else if (value.compare("vt") == 0) {
				//std::cout << "adding face" << std::endl;
				std::string tmp = ss.str().substr(2);
				handleTexture(m, tmp);
			}
		}
	}
#endif
	//m->print();

	AbstractModel* a = m;
	return a;
}

void WavefrontLoader::handleVertex(WavefrontModel* m, std::string s) {
	std::istringstream ss;

	vector<string> strs;
	vector<float> vertices;

	splitString(s, strs, " ");

	for (int i = 0; i < strs.size(); i++) {
		float f = 0;
		ss.clear();
		ss.str(strs.at(i));
		ss >> f;
		vertices.push_back(f);
	}

	m->addVertex(vertices.at(0), vertices.at(1), vertices.at(2));
}

void WavefrontLoader::handleNormal(WavefrontModel* m, std::string s) {
	std::istringstream ss;

	vector<string> strs;
	vector<float> normals;

	splitString(s, strs, " ");

	for (int i = 0; i < strs.size(); i++) {
		float f = 0;
		ss.clear();
		ss.str(strs.at(i));
		ss >> f;
		normals.push_back(f);
	}

	m->addNormal(normals.at(0), normals.at(1), normals.at(2));
}

void WavefrontLoader::handleTexture(WavefrontModel* m, std::string s) {
	std::istringstream ss;

	vector<string> strs;
	vector<float> textures;

	splitString(s, strs, " ");

	for (int i = 0; i < strs.size(); i++) {
		float f = 0;
		ss.clear();
		ss.str(strs.at(i));
		ss >> f;
		textures.push_back(f);
	}

	m->addTexture(textures.at(0), textures.at(1), textures.at(2));
}

void WavefrontLoader::handleFace(WavefrontModel* m, std::string s) {
	int i = s.find("/");

	vector<int> vertices;
	vector<int> normals;
	vector<int> textures;

	// if there are /'s in the string, which means there are more than just vertices in it
	if (i != -1) {
		i = s.find("//");

		if (i != -1) {
			// that means that there isn't a texture in the string
			std::istringstream ss;

			vector<string> strs;

			splitString(s, strs, " ");

			for (int x = 0; x < strs.size(); x++) {
				vector<string> tmp;
				splitString(strs.at(x), tmp, "//");

				for (int k = 0; k < tmp.size(); k++) {

					//std::cout << "// x is " << tmp.at(k) << std::endl;

					int f = 0;
					ss.clear();
					ss.str(tmp.at(k));
					ss >> f;
					vertices.push_back(f);

					k++;
					ss.clear();
					ss.str(tmp.at(k));
					ss >> f;
					normals.push_back(f);

					textures.push_back(0);
				}
			}
		} else {
			// this means there is a texture in the string
			std::istringstream ss;

			vector<string> strs;

			splitString(s, strs, " ");

			for (int x = 0; x < strs.size(); x++) {
				vector<string> tmp;
				splitString(strs.at(x), tmp, "/");

				for (int k = 0; k < tmp.size(); k++) {

					//std::cout << "/ x is " << tmp.at(k) << std::endl;

					int f = 0;
					ss.clear();
					ss.str(tmp.at(k));
					ss >> f;
					vertices.push_back(f);
					k++;

					ss.clear();
					ss.str(tmp.at(k));
					ss >> f;
					textures.push_back(f);
					k++;

					ss.clear();
					ss.str(tmp.at(k));
					ss >> f;
					normals.push_back(f);

				}
			}
		}
	} else {
		// this means that there are only vertices in the string
		std::istringstream ss;

		vector<string> strs;

		splitString(s, strs, " ");

		for (int x = 0; x < strs.size(); x++) {
			int f = 0;
			ss.clear();
			ss.str(strs.at(x));
			ss >> f;
			vertices.push_back(f);
			normals.push_back(0);
			textures.push_back(0);
		}
	}
	m->addFace(vertices, textures, normals);
}
