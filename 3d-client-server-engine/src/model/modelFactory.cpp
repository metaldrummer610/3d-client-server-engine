#include <sstream>
#include <iostream>
#include <vector>

#include "modelFactory.h"
#include "../util/common.h"

AbstractModel* ModelFactory::getModel(std::string s) {

	std::string name;
	int id;
	float x;
	float y;
	float z;
	float angleX;
	float angleY;
	float angleZ;

	std::vector<std::string> args;

	splitString(s, args, ",");

	std::istringstream ss;

	ss.str(args[0]);
	ss >> name;

	ss.clear();
	ss.str(args[1]);
	ss >> id;

	ss.clear();
	ss.str(args[2]);
	ss >> x;

	ss.clear();
	ss.str(args[3]);
	ss >> y;

	ss.clear();
	ss.str(args[4]);
	ss >> z;

	ss.clear();
	ss.str(args[5]);
	ss >> angleX;

	ss.clear();
	ss.str(args[6]);
	ss >> angleY;

	ss.clear();
	ss.str(args[7]);
	ss >> angleZ;

	if (name.compare("cube") == 0) {
		AbstractModel* a = new Cube();

		a->setId(id);
		a->setX(x);
		a->setY(y);
		a->setZ(z);
		a->setAngleX(angleX);
		a->setAngleY(angleY);
		a->setAngleZ(angleZ);

		return a;
	} else if (name.compare("pyramid") == 0) {
		AbstractModel* a = new Pyramid();

		a->setId(id);
		a->setX(x);
		a->setY(y);
		a->setZ(z);
		a->setAngleX(angleX);
		a->setAngleY(angleY);
		a->setAngleZ(angleZ);

		return a;
	} else if (name.find(".obj") != -1) {
		AbstractModel* a;
		a = l.load(name.c_str());

		a->setId(id);
		a->setX(x);
		a->setY(y);
		a->setZ(z);
		a->setAngleX(angleX);
		a->setAngleY(angleY);
		a->setAngleZ(angleZ);

		return a;
	}
}

AbstractModel* ModelFactory::getModelByName(std::string name) {
	if(name.compare("cube") == 0) {
		return new Cube();
	} else if (name.compare("pyramid") == 0) {
		return new Pyramid();
	} else if (name.find(".obj") != -1) {
		AbstractModel* a;
		a = l.load(name.c_str());
		return a;
	}
}

void ModelFactory::updateCoords(AbstractModel* m, std::string str) {
	std::string name;
	int id;
	float x;
	float y;
	float z;
	float angleX;
	float angleY;
	float angleZ;

	std::vector<std::string> args;

	splitString(str, args, ",");

	std::istringstream ss;

	ss.str(args[0]);
	ss >> name;

	ss.clear();
	ss.str(args[1]);
	ss >> id;

	ss.clear();
	ss.str(args[2]);
	ss >> x;

	ss.clear();
	ss.str(args[3]);
	ss >> y;

	ss.clear();
	ss.str(args[4]);
	ss >> z;

	ss.clear();
	ss.str(args[5]);
	ss >> angleX;

	ss.clear();
	ss.str(args[6]);
	ss >> angleY;

	ss.clear();
	ss.str(args[7]);
	ss >> angleZ;

	m->setId(id);
	m->setX(x);
	m->setY(y);
	m->setZ(z);
	m->setAngleX(angleX);
	m->setAngleY(angleY);
	m->setAngleZ(angleZ);
}
