#ifndef _CUBE_H
#define _CUBE_H
#include <string>
#include "abstractModel.h"
using namespace std;

class Cube: public AbstractModel {
public:
	Cube();
	void draw();
	string serialize();
	void deserialize(string& s);
};
#endif
