/*
 * wavefrontModel.h
 *
 *  Created on: Sep 13, 2008
 *      Author: metal
 */

#ifndef WAVEFRONTMODEL_H_
#define WAVEFRONTMODEL_H_

#include <vector>
#include <string>
#include <SDL/SDL_opengl.h>
#include "../abstractModel.h"

typedef struct {
	float x;
	float y;
	float z;
} Vertex;

typedef struct {
	int vertex;
} Point;

typedef struct {
	std::vector<Point> points;
} Face;

class WaveFrontModel: public AbstractModel {
private:
	std::vector<Face> faces;
	std::vector<Vertex> verticies;
public:
	WaveFrontModel(std::string& s);
	void addFace(std::vector<int> vertexPos);
	void addVertex(float x, float y, float z);
	void draw();
	std::string serialize();
	void deserialize(std::string& s);
};

#endif /* WAVEFRONTMODEL_H_ */
