#ifndef _MODEL_FACTORY_H
#define _MODEL_FACTORY_H
#include <string>
using namespace std;

#include "abstractModel.h"
#include "cube.h"
#include "pyramid.h"

class ModelFactory {
private:
	static Cube* cube;
	static Pyramid* pyramid;
public:
	AbstractModel* getModel(string s);
	AbstractModel* getModelByName(string name);
};
#endif
