#ifndef _MODEL_FACTORY_H
#define _MODEL_FACTORY_H
#include <string>

#include "abstractModel.h"
#include "cube.h"
#include "pyramid.h"

class ModelFactory {
private:
	static Cube* cube;
	static Pyramid* pyramid;
public:
	AbstractModel* getModel(std::string s);
	AbstractModel* getModelByName(std::string name);
};
#endif
