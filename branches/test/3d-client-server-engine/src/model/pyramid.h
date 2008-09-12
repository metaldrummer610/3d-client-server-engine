#ifndef _PYRAMID_H
#define _PYRAMID_H
#include <string>
#include "abstractModel.h"

class Pyramid: public AbstractModel {
private:
	float angle;
public:
	Pyramid();
	void draw();
	std::string serialize();
	void deserialize(std::string& s);
};
#endif
