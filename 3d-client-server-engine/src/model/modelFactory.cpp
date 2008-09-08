#include <sstream>
using namespace std;

#include "modelFactory.h"

Cube* ModelFactory::cube = new Cube();
Pyramid* ModelFactory::pyramid = new Pyramid();


AbstractModel* ModelFactory::getModel(string s) {

	string::iterator it;
	string temp;
	temp = "";

	string name;
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
			//cout << "c is: " << *it << ":" << endl;
			temp.append(&c);
		} else {
			//cout << "temp is: " << temp << endl;
			istringstream in(temp);

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
	}
}

AbstractModel* ModelFactory::getModelByName(string name) {
	if (name.compare("cube") == 0) {
		return cube;
	} else if (name.compare("pyramid") == 0) {
		return pyramid;
	}
}
