#include <iostream>

#include <GL/glew.h>

// GLFW
#include <glfw3.h>

// Other Libs
#include <SOIL.h>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// Other includes
#include "Shader.h"
#include "testcube.h"
#include <string>
using namespace std;
GLFWwindow* gl_glfw_init();

const GLuint WIDTH = 800, HEIGHT = 600;

glm::vec3 cubevex[4]=
{
	glm::vec3( 0.5f, 0.5f, 0.0f),
	glm::vec3(-0.5f, 0.5f, 0.0f),
	glm::vec3(-0.5f,-0.5f, 0.0f),
	glm::vec3( 0.5f,-0.5f, 0.0f),
};
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
testcube cube;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	GLfloat incre = 0.04;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		cube.moveedge(incre,'y','+');
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		cube.moveedge(incre,'y','-');
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		cube.moveedge(incre,'x','+');
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		cube.moveedge(incre,'x','-');
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		cube.changetype(SCALE);
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		cube.changetype(CURRENT);
	}
}

int main()
{
	GLFWwindow* window;
	window = gl_glfw_init();
    // Build and compile our shader program
	cube.loadvertex(cubevex);
	string surfaceshader[]={ "vertex.txt", "", "fragment.txt"};
	string elemshader1[]={ "ins_vertex.txt", "geometry.txt", "fragment2.txt"};
	string elemshader2[]={ "ins_vertex.txt", "", "fragment.txt"};

	cube.initShader(surfaceshader,elemshader1,elemshader2);
	cube.init();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cube.drawcube(model, view, projection);

        glfwSwapBuffers(window);
    }
    glfwTerminate();



    return 0;
}


// Is called whenever a key is pressed/released via GLFW
GLFWwindow* gl_glfw_init()
{
	  // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
    // Set the required callback functions
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);
    // Setup OpenGL options
    glEnable(GL_DEPTH_TEST);
	
	return window;
}
