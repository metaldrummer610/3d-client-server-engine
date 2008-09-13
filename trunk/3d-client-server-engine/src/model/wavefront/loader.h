/*
 * loader.h
 *
 *  Created on: Sep 13, 2008
 *      Author: metal
 */

#ifndef LOADER_H_
#define LOADER_H_

#include <string>
#include "wavefrontModel.h"
#include "../abstractModel.h"

class Loader {
private:
	void addFace(std::string s, WaveFrontModel* m);
	void addVertex(std::string s, WaveFrontModel* m);
public:
	AbstractModel* loadFile(std::string &s);
};

#endif /* LOADER_H_ */
