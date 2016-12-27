#include "testcube.h"

testcube::testcube(glm::vec3 vertices[])
{
	for(int i = 0; i<4;i++)
	{
		_vertex[i]= vertices[i];
	}
}

void testcube::init()//const GLchar* vertexPath,const GLchar* ins_vertexPath, const GLchar* fragmentPath)
{
	GLfloat vertextmp[3 * 4];
	int k = 0;
	for(int i =0; i<4 ; i++)
	{
		vertextmp[k++]=_vertex[i].x;
		vertextmp[k++]=_vertex[i].y;
		vertextmp[k++]=_vertex[i].z;
	}
//	surface = gl_color_ver_primitive();
	surface.init(4,vertextmp, glm::vec3(1.0f, 0.5f, 0.2f),glm::mat4());
	surface.writebuffer();

	ele_color.init(glm::mat4(),SCALE);
	GLfloat orign[] = {_vertex[2].x,_vertex[2].y,_vertex[2].z};

	ele_color.react_vertexchange(orign,_vertex[0]-_vertex[1],_vertex[1] - _vertex[2]);
}

void testcube::initShader(std::string surfaceshader[],std::string elemshader1[], std::string elemshader2[])
{
	if(surfaceshader[1].length()==0)
		surface.initshader((GLchar *)surfaceshader[0].c_str(),NULL,(GLchar * )surfaceshader[2].c_str());
	else
		surface.initshader((GLchar *)surfaceshader[0].c_str(),(GLchar * )surfaceshader[1].c_str(),(GLchar * )surfaceshader[2].c_str());
	
	if(elemshader1[1].length()==0 && elemshader2[1].length()==0)
		ele_color.initshader((GLchar *)elemshader1[0].c_str(),NULL,(GLchar *)elemshader1[2].c_str(),(GLchar *)elemshader2[0].c_str(),NULL,(GLchar *)elemshader2[2].c_str());
	else if(elemshader1[1].length()==0)
		ele_color.initshader((GLchar *)elemshader1[0].c_str(),NULL,(GLchar *)elemshader1[2].c_str(),(GLchar *)elemshader2[0].c_str(),(GLchar *)elemshader2[1].c_str(),(GLchar *)elemshader2[2].c_str());
	else if(elemshader2[1].length()==0)
		ele_color.initshader((GLchar *)elemshader1[0].c_str(),(GLchar *)elemshader1[1].c_str(),(GLchar *)elemshader1[2].c_str(),(GLchar *)elemshader2[0].c_str(),NULL,(GLchar *)elemshader2[2].c_str());
	else
		ele_color.initshader((GLchar *)elemshader1[0].c_str(),(GLchar *)elemshader1[2].c_str(),(GLchar *)elemshader1[2].c_str(),(GLchar *)elemshader2[0].c_str(),(GLchar *)elemshader2[1].c_str(),(GLchar *)elemshader2[2].c_str());
}

void testcube::drawcube(glm::mat4 const & model, glm::mat4 const & view, glm::mat4 const &projection)
{
	surface.changemodel(model);
	surface.draw(view,projection);

	ele_color.changemodel(model);
	ele_color.draw(view,projection);
}

void testcube::moveedge(GLfloat incre, char axis, char dir)
{
	switch (axis)
	{
	case 'x':
		if(dir == '+')
		{
			_vertex[0].x += incre;
			_vertex[3].x += incre;
		}
		else
		{
			_vertex[1].x -= incre;
			_vertex[2].x -= incre;
		}
		break;
	case 'y':
		if(dir == '+')
		{
			_vertex[0].y += incre;
			_vertex[1].y += incre;
		}
		else
		{
			_vertex[2].y -= incre;
			_vertex[3].y -= incre;
		}
		break;
	default:
		break;
	}

	GLfloat vertextmp[3 * 4];
	int k = 0;
	for(int i =0; i<4 ; i++)
	{
		vertextmp[k++]=_vertex[i].x;
		vertextmp[k++]=_vertex[i].y;
		vertextmp[k++]=_vertex[i].z;
	}
	surface.changevertex(vertextmp);
	GLfloat orign[] = {_vertex[2].x,_vertex[2].y,_vertex[2].z};

	ele_color.react_vertexchange(orign,_vertex[0]-_vertex[1],_vertex[1] - _vertex[2]);
}

void testcube::loadvertex(glm::vec3 vex[])
{
	for(int i =0; i < 4 ; i++)
	{
		_vertex[i] = vex[i];
	}
}

void testcube::changetype(GLuint rendertype)
{
	ele_color.react_changetype(rendertype);
}