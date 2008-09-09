#include "md2.h"

vector_t Md2Model::operator-(vector_t a, vector_t b) {
	vector_t c;

	c.point[0] = a.point[0] - b.point[0];
	c.point[1] = a.point[1] - b.point[1];
	c.point[2] = a.point[2] - b.point[2];

	return c;
}

vector_t Md2Model::operator*(float f, vector_t b) {
	vector_t c;

	c.point[0] = f * b.point[0];
	c.point[1] = f * b.point[1];
	c.point[2] = f * b.point[2];

	return c;
}

vector_t Md2Model::operator/(vector_t a, vector_t b) {
	vector_t c;

	c.point[0] = a.point[0] / b.point[0];
	c.point[1] = a.point[1] / b.point[1];
	c.point[2] = a.point[2] / b.point[2];

	return c;
}

vector_t Md2Model::operator+(vector_t a, vector_t b) {
	vector_t c;

	c.point[0] = a.point[0] + b.point[0];
	c.point[1] = a.point[1] + b.point[1];
	c.point[2] = a.point[2] + b.point[2];

	return c;
}
