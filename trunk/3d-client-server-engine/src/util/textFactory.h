#ifndef _TEXT_FACTORY_H
#define _TEXT_FACTORY_H
#include <GL/gl.h>

class TextFactory {
private:
	GLuint base; /* Base Display List For The Font Set */
public:
	void buildFont();
	void glPrint(const char *fmt, ...);
	void killFont();
};
#endif
