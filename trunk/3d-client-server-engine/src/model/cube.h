#ifndef _CUBE_H
#define _CUBE_H
#include <string>
#include "abstractModel.h"

class Cube: public AbstractModel {
public:
	Cube();
	void draw();
	std::string serialize();
	void deserialize(std::string& s);
};
#endif
