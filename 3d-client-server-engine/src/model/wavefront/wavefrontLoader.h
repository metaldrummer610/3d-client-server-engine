/*
 * wavefrontLoader.h
 *
 *  Created on: Sep 16, 2008
 *      Author: diazr
 */

#ifndef WAVEFRONTLOADER_H_
#define WAVEFRONTLOADER_H_

#include <sstream>
#include <string>
#include "wavefrontModel.h"

class WavefrontLoader {
private:
	void handleVertex(WavefrontModel* m, std::string s);
	void handleNormal(WavefrontModel* m, std::string s);
	void handleTexture(WavefrontModel* m, std::string s);
	void handleFace(WavefrontModel* m, std::string s);
public:
	WavefrontModel* load(const char* str);
};

#endif /* WAVEFRONTLOADER_H_ */
