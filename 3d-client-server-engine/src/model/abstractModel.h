#ifndef _ABSTRACT_MODEL_H
#define _ABSTRACT_MODEL_H
#include <string>

class AbstractModel {
protected:
	float x;
	float y;
	float z;
	int id;
	std::string name;
public:
	virtual void draw() = 0;
	virtual std::string serialize() = 0;
	virtual void deserialize(std::string& s) = 0;

	float getX() {return x;}
	float getY() {return y;}
	float getZ() {return z;}
	int getId() {return id;}
	std::string getName() {return name;}

	void setX(float f) {x = f;}
	void setY(float f) {y = f;}
	void setZ(float f) {z = f;}
	void setId(int i) {id = i;}
};
#endif
