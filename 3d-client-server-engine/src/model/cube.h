#ifndef _CUBE_H
#define _CUBE_H
#include <string>
#include "abstractModel.h"

class Cube: public AbstractModel {
private:
	float angle;
public:
	Cube();
	void draw();
	std::string serialize();
};
#endif
