#ifndef _MODEL_FACTORY_H
#define _MODEL_FACTORY_H
#include <string>

#include "abstractModel.h"
#include "cube.h"
#include "pyramid.h"

#include "wavefront/loader.h"

class ModelFactory {
private:
	std::string sphereFilePath;
	WavefrontLoader wfLoader;
public:
	ModelFactory();
	AbstractModel* getModel(std::string s);
	AbstractModel* getModelByName(std::string name);
};
#endif
