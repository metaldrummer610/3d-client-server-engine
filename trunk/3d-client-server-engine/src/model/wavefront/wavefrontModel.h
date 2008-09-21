/*
 * wavefrontModel.h
 *
 *  Created on: Sep 16, 2008
 *      Author: diazr
 */

#ifndef WAVEFRONTMODEL_H_
#define WAVEFRONTMODEL_H_

#include <vector>
#include <string>
#include "../abstractModel.h"

using std::vector;

typedef struct {
	float x;
	float y;
	float z;
} Vertex;

typedef struct {
	float i;
	float j;
	float k;
} Normal;

typedef struct{
	float x;
	float y;
	float z;
} Texture;

typedef struct {
	int vertex;
	int texture;
	int normal;
} Point;

typedef struct {
	vector<Point> points;
} Face;

/**
 * faces hold a number of points. each point holds the index of the vectors for the vertex, texture and normal.
 */

class WavefrontModel : public AbstractModel{
private:
	vector<Vertex> vertices;
	vector<Normal> normals;
	vector<Texture> textures;
	vector<Face> faces;
public:
	void addVertex(float x, float y, float z);
	void addNormal(float i, float j, float k);
	void addTexture(float x, float y, float z);
	void addFace(vector<int> vertices, vector<int> textures, vector<int> normals);
	void draw();
	std::string serialize();
	void print();
};

#endif /* WAVEFRONTMODEL_H_ */
