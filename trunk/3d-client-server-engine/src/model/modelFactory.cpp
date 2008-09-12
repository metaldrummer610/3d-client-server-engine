#include <sstream>
#include <iostream>

#include "modelFactory.h"

ModelFactory::ModelFactory() {
	sphereFilePath = "resources/models/ILOVELAMP.obj";
}

AbstractModel* ModelFactory::getModel(std::string s) {

	std::string::iterator it;
	std::string temp;
	temp = "";

	std::string name;
	int id;
	float x;
	float y;
	float z;

	bool nameDone = false;
	bool idDone = false;
	bool xDone = false;
	bool yDone = false;
	bool zDone = false;

	for (it = s.begin(); it != s.end(); it++) {
		if (*it != ',') {
			char c = *it;
			//std::cout << "c is: " << *it << ":" << std::endl;
			temp.append(&c);
		} else {
			//std::cout << "temp is: " << temp << std::endl;
			std::istringstream in(temp);

			if (nameDone == false) {
				in >> name;
				nameDone = true;
			} else if (idDone == false) {
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

	if (name.compare("cube") == 0) {
		AbstractModel* a = new Cube();

		a->setId(id);
		a->setX(x);
		a->setY(y);
		a->setZ(z);

		return a;
	} else if (name.compare("pyramid") == 0) {
		AbstractModel* a = new Pyramid();

		a->setId(id);
		a->setX(x);
		a->setY(y);
		a->setZ(z);

		return a;
	} else if (name.find("resources/models") != -1) {
		AbstractModel* a = wfLoader.load(name.c_str());

		a->setId(id);
		a->setX(x);
		a->setY(y);
		a->setZ(z);

		return a;
	}
}

AbstractModel* ModelFactory::getModelByName(std::string name) {
	if (name.compare("cube") == 0) {
		return new Cube();
	} else if (name.compare("pyramid") == 0) {
		return new Pyramid();
	} else if (name.compare("sphere") == 0) {
		return wfLoader.load(sphereFilePath.c_str());
	}
}
