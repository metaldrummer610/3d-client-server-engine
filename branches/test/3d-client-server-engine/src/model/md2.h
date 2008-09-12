#ifndef _MD2_H
#define _MD2_H

typedef struct {
	int ident;
	int version;
	int skinWidth;
	int skinHeight;
	int frameSize;
	int numSkins;
	int numXYZ;
	int numST;
	int numTris;
	int numGLcmds;
	int numFrames;
	int offsetSkins;
	int offsetST;
	int offsetTris;
	int offsetFrames;
	int offsetGLcmds;
	int offsetEnd;
} modelHeader_t;

// a single vector
typedef struct {
	float point[3];
} vector_t;

// texture coordinate
typedef struct {
	float s;
	float t;
} texCoord_t;

// texture coordinate index
typedef struct {
	short s;
	short t;
} stIndex_t;

// info for a single frame point
typedef struct {
	unsigned char v[3];
	unsigned char normalIndex;
} framePoint_t;

// information for a single frame
typedef struct {
	float scale[3]; // scaling for the frame verticies
	float translate[3]; // translation for frame verticies
	char name[16]; // name of model
	framePoint_t fp[1]; // beginning of the frame vertex list
} frame_t;

// data for a single triangle
typedef struct {
	unsigned short meshIndex[3]; // vertex indicies
	unsigned short stIndex[3]; // texture-coordinate indicies
} mesh_t;



class Md2Model: public AbstractModel {
private:

public:
	Md2Model();
	void draw();
	std::string serialize();
	void deserialize(std::string& s);

	vector_t operator-(vector_t a, vector_t b);
	vector_t operator*(float f, vector_t b);
	vector_t operator/(vector_t a, vector_t b);
	vector_t operator+(vector_t a, vector_t b);
};

#endif
