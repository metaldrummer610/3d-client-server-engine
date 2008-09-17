#ifndef _MODEL_FACTORY_H
#define _MODEL_FACTORY_H
#include <string>

#include "abstractModel.h"
#include "cube.h"
#include "pyramid.h"
#include "wavefront/wavefrontLoader.h"

class ModelFactory {
private:
	WavefrontLoader l;
public:
	AbstractModel* getModel(std::string s);
	AbstractModel* getModelByName(std::string name);
};
#endif
