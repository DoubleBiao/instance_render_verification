#ifndef TESTCUBE
#define TESTCUBE

#include "globject.h"
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class testcube
{
private:
	glm::vec3 _vertex[4];
	gl_color_ver_primitive surface;
	gl_color_ver_instance_primitive ele_color;
public:
	testcube(){};
	testcube(glm::vec3 vertices[]);
	void loadvertex(glm::vec3 vex[]);
	void init(const GLchar* vertexPath,const GLchar* ins_vertexPath, const GLchar* fragmentPath);
	void moveedge(GLfloat incre, char axis, char dir);
	void drawcube(glm::mat4 const & model, glm::mat4 const & view, glm::mat4 const &projection);
};

#endif