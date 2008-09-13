#ifndef _MODEL_FACTORY_H
#define _MODEL_FACTORY_H
#include <string>

#include "abstractModel.h"
#include "cube.h"
#include "pyramid.h"
#include "wavefront/loader.h"

class ModelFactory {
private:
	Loader l;
public:
	AbstractModel* getModel(std::string s);
	AbstractModel* getModelByName(std::string name);
};
#endif
