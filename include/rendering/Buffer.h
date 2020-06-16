#include "GL/glew.h"
#include <string>


class Buffer {

private:
	GLuint bufferID;

public:
	Buffer(GLenum datatype, int size);
	~Buffer();
	GLuint getID();

}
