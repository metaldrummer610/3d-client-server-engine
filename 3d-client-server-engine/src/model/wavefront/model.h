#ifndef WAVEFRONT_MODEL_H
#define WAVEFRONT_MODEL_H

#include <vector>
#include <list>

#include <boost/shared_ptr.hpp>
#include <sstream>

#include "../abstractModel.h"

struct Vertex {
	float x;
	float y;
	float z;
	float w;
};

struct Normal {
	float i;
	float j;
	float k;
};

struct Point {
	int vertex;
	int normal;
	int texture;
};

struct Face {
	std::vector<Point> points;
};

class WavefrontRenderer;

/**
 * The wavefront model storage object.
 */
class WavefrontModel: public AbstractModel {
public:
	friend class WavefrontLoader;
	friend class WavefrontRenderer;

	WavefrontModel();

	inline void addVertex(float x, float y, float z, float w) {
		vertexCount++;
		Vertex v = { x, y, z, w };
		vertices.push_back(v);
	}

	template<typename InputIterator>
	void addFace(InputIterator vbegin, InputIterator vend,
			InputIterator fbegin, InputIterator fend, InputIterator nbegin,
			InputIterator nend) {
		faceCount++;
		Face f;
		Point p = { 0, 0, 0 };
		for (; vbegin != vend; ++vbegin) {
			p.vertex = *vbegin;
			f.points.push_back(p);
		}
		for (int i = 0; fbegin != fend; i++, ++fbegin) {
			f.points[i].texture = *fbegin;
		}
		for (int i = 0; nbegin != nend; i++, ++nbegin) {
			f.points[i].normal = *nbegin;
		}
		faces.push_back(f);
	}

	inline void addNormal(float i, float j, float k) {
		normalCount++;
		Normal n = { i, j, k };
		normals.push_back(n);
	}

	void draw();
	std::string serialize();
	void deserialize(std::string& s);
protected:
	void drawFace(Face& f);
	void drawVertex(Point& p);

	std::vector<Vertex> vertices;
	std::vector<Face> faces;
	std::vector<Normal> normals;

	boost::shared_ptr<WavefrontRenderer> renderer;

	int vertexCount;
	int faceCount;
	int normalCount;
private:
};

#endif//WAVEFRONT_MODEL_H
