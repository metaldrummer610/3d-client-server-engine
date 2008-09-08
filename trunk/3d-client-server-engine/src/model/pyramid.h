#ifndef _PYRAMID_H
#define _PYRAMID_H
#include <string>
#include "abstractModel.h"
using namespace std;

class Pyramid: public AbstractModel {
public:
	Pyramid();
	void draw();
	string serialize();
	void deserialize(string& s);
};
#endif
