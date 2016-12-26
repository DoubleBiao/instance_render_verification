
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
// GLFW
#include <glfw3.h>
#include<iostream>
#include<vector>
#include"shader.h"

#ifndef GLOBJECT
#define GLOBJECT


class glprimitive
{
protected:
	Shader _shader;
	glm::mat4 _view;
	glm::mat4 _projection;
public:
	void initshader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar*geometrypath)
	{
		if(geometrypath != NULL)
		{
			_shader = Shader(vertexPath, fragmentPath, geometrypath);
		}
		else
		{
			_shader = Shader(vertexPath, fragmentPath);
		}
	}
	virtual void setattributepointer() = 0;
	virtual void writeobjectanduniform() = 0;
	virtual void bindbuffers() = 0;
	virtual void unbindbuffers() = 0;
	virtual void dorendering() = 0;

	void draw(glm::mat4 const & view, glm::mat4 const & projection)
	{
		_view = view;
		_projection = projection;
		_shader.Use();
		setattributepointer();
		writeobjectanduniform();
		bindbuffers();
		dorendering();
		unbindbuffers();
	}
};

class gl_color_ver_primitive: public glprimitive
{
protected:
	GLuint _VAO;
	GLuint _VBO;
	GLuint _verticesnum;
	GLfloat *_vertex;
	glm::vec3 _color;
	GLuint _buffer_state;
	GLuint _mode;
	glm::mat4 _model;
public:
	gl_color_ver_primitive(){_vertex = NULL;}
	~gl_color_ver_primitive()
	{
		delete _vertex;
		glDeleteBuffers(1, &_VBO);
		glDeleteVertexArrays(1, &_VAO);
	}
virtual	void writebuffer()
		{
			GLfloat *colortmp;
			colortmp = new GLfloat[_verticesnum * 3];


			int k = 0;
			for(int i = 0; i<_verticesnum; i++)
			{
				colortmp[k++] = _color.x;
				colortmp[k++] = _color.y;
				colortmp[k++] = _color.z;
			}

			glBindVertexArray(_VAO);
			glBindBuffer(GL_ARRAY_BUFFER, _VBO);
			GLuint offset = 0;
			glBufferSubData(GL_ARRAY_BUFFER, offset, _verticesnum *3 * sizeof(GLfloat), _vertex);
			offset +=  _verticesnum *3 * sizeof(GLfloat);
			glBufferSubData(GL_ARRAY_BUFFER, offset, _verticesnum *3 * sizeof(GLfloat), colortmp);

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	void init(GLuint vertexnum, GLfloat vertex[], glm::vec3 &color,
		const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar*geometrypath, glm::mat4 const & model,
		GLuint mode,GLuint buffer_state = GL_STATIC_DRAW)
	{
		_model = model;
		_mode  = mode;
		initshader(vertexPath,fragmentPath,geometrypath);
		GLuint k = 0;
		_verticesnum = vertexnum;

		_vertex = new float[_verticesnum * 3];

		if(vertex != NULL)
		{
			for(int i = 0; i<_verticesnum; i++)
			{
				for(int j = 0; j <3 ; j++)
				{
					_vertex[k] = vertex[k++];
				}
			}
		}

		_color = color;
		_buffer_state = buffer_state;

		glGenVertexArrays(1, &_VAO);
		glGenBuffers(1, &_VBO);
	
		glBindVertexArray(_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glBufferData(GL_ARRAY_BUFFER, _verticesnum*3*sizeof(GLfloat)*2, NULL, _buffer_state);   // vertices position and color

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(_verticesnum*3*sizeof(GLfloat)));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

//		writebuffer();
	}
	void writeobjectanduniform()
	{
		//GLint modelLoc = glGetUniformLocation(_shader.Program, "model");
        //GLint viewLoc = glGetUniformLocation(_shader.Program, "view");
        //GLint projLoc = glGetUniformLocation(_shader.Program, "projection");
        // Pass them to the shaders
        //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(_model));
        //glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(_view));
        //glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(_projection));
	}
	void bindbuffers()
	{
		glBindVertexArray(_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	}
	void unbindbuffers()
	{
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void dorendering()
	{
		glDrawArrays(_mode, 0, _verticesnum);
	}
	void setattributepointer(){}
	void changecolor(glm::vec3 const & color)
	{
		_color = color;
		bindbuffers();
		writebuffer();
		unbindbuffers();
	}
virtual	void changevertex(GLfloat * vertex)
		{
			GLuint k = 0;

			//_vertex = new float[_verticesnum * 3];

			for(int i = 0; i<_verticesnum; i++)
			{
				for(int j = 0; j <3 ; j++)
				{
					_vertex[k++] = vertex[k];
				}
			}
			bindbuffers();
			writebuffer();
			unbindbuffers();
		}
	void changemodel(glm::mat4 model)
	{
		_model = model;
	}
};

class gl_color_ver_instance_primitive: public gl_color_ver_primitive
{
protected:
	GLuint _rownum;
	GLfloat _elem_dimen;
	GLuint _elemennum;
	glm::vec3 _rowdir;
	glm::vec3 _columndir;

public:
	void writebuffer()
	{
		glBindVertexArray(_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, _verticesnum *3 * sizeof(GLfloat), _vertex);
	
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void react_vertexchange(GLfloat orgine[], glm::vec3 rowdir, glm::vec3 columndir)
	{
		float rowlen = glm::length(rowdir);
		float columnlen = glm::length(columndir);
		
		_rownum = rowlen/_elem_dimen;
		_elemennum = (rowlen/_elem_dimen)*(columnlen /_elem_dimen);
		_rowdir = rowdir;
		_columndir = columndir;

		GLfloat vertex_tmp[12];
		                                                                 //         elementsquare:
		vertex_tmp[0 + 2 * 3] = orgine[0];                               //       1   --rowdir-->   0
		vertex_tmp[1 + 2 * 3] = orgine[1];                               //      / \               / \ 
		vertex_tmp[2 + 2 * 3] = orgine[2];                               //       |                 |
		                                                                 //    colunmdir         columndir
		vertex_tmp[0 + 0 * 3] = orgine[0] + columndir.x * _elem_dimen;   //       |                 |
		vertex_tmp[1 + 0 * 3] = orgine[1] + columndir.y * _elem_dimen;   //       |                 |
		vertex_tmp[2 + 0 * 3] = orgine[2] + columndir.z * _elem_dimen;   //       2   --rowdir-->   3 

		vertex_tmp[0 + 3 * 3] = orgine[0] + rowdir.x * _elem_dimen;  
		vertex_tmp[1 + 3 * 3] = orgine[1] + rowdir.y * _elem_dimen;
		vertex_tmp[2 + 3 * 3] = orgine[2] + rowdir.z * _elem_dimen;

		vertex_tmp[0 + 1 * 3] = orgine[0] + rowdir.x * _elem_dimen + columndir.x * _elem_dimen;  
		vertex_tmp[1 + 1 * 3] = orgine[1] + rowdir.y * _elem_dimen + columndir.y * _elem_dimen;
		vertex_tmp[2 + 1 * 3] = orgine[2] + rowdir.z * _elem_dimen + columndir.z * _elem_dimen;

		changevertex(vertex_tmp);
	}
	void dorendering()
	{
		glDrawArraysInstancedARB(_mode, 0, _verticesnum,_elemennum);
	}
	void init(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar*geometrypath, glm::mat4 const & model,
		GLuint mode,GLuint buffer_state = GL_STATIC_DRAW)
	{
		_model = model;
		_mode  = mode;
		_elem_dimen = .001;
		initshader(vertexPath,fragmentPath,geometrypath);
		GLuint k = 0;
		_verticesnum = 4;

		_vertex = new float[_verticesnum * 3];

		_buffer_state = buffer_state;

		glGenVertexArrays(1, &_VAO);
		glGenBuffers(1, &_VBO);
	
		glBindVertexArray(_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glBufferData(GL_ARRAY_BUFFER, _verticesnum*3*sizeof(GLfloat), NULL, _buffer_state);   // vertices position and color

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void writeobjectanduniform()
	{
		//GLint modelLoc = glGetUniformLocation(_shader.Program, "model");
        //GLint viewLoc = glGetUniformLocation(_shader.Program, "view");
        //GLint projLoc = glGetUniformLocation(_shader.Program, "projection");
        // Pass them to the shaders
        //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(_model));
        //glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(_view));
        //glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(_projection));

		GLint rowdirLoc = glGetUniformLocation(_shader.Program, "rowdir");
		GLint columndirLoc = glGetUniformLocation(_shader.Program, "columndir");
		GLint rownumLoc = glGetUniformLocation(_shader.Program, "rownum");
		GLint eledimensionLoc = glGetUniformLocation(_shader.Program, "eledimension");
		
		glUniform3fv(rowdirLoc, 1, glm::value_ptr(glm::normalize(_rowdir)));
		glUniform3fv(columndirLoc, 1, glm::value_ptr(glm::normalize(_columndir)));
		glUniform1i(rownumLoc, _rownum);
		glUniform1f(eledimensionLoc,_elem_dimen);
};
};
class baseobject
{
//protected:
//public:
//	glpaint *_paintobject;
//	int _vextexnum;
//	int _partnum;
//	glm::vec3 * _vertices;
//	globject_part * _part;
//public:
//	baseobject(){_vertices = NULL; _part = NULL;_paintobject = NULL;};
//	baseobject(glm::vec3  ver[], int vertexnum,globject_part part[], int partnum)
//	{
//		_paintobject = new glpaint[partnum];
//		
//		for(int i =0; i< partnum;i++)
//		{
//			_paintobject[i].setbuffer(1);
//		}
//
//		_vextexnum = vertexnum;
//		_partnum = partnum;
//		_vertices = new glm::vec3[vertexnum];
//		_part = new globject_part[partnum];
//		for(int i = 0; i < vertexnum; i++)
//		{
//			_vertices[i] = ver[i];
//		}
//		for(int i = 0 ;i < partnum; i++)
//		{
//			_part[i] = part[i];
//		}
//	}
//	void init_buffer(int partnum)
//	{
//		_paintobject = new glpaint[partnum];
//	
//		for(int i =0; i< partnum;i++)
//		{
//			_paintobject[i].setbuffer(1);
//		}
//	}
//	void init_buffer()
//	{
//		_paintobject = new glpaint[_partnum];
//	
//		for(int i =0; i< _partnum;i++)
//		{
//			_paintobject[i].setbuffer(1);
//		}
//	}
//	void set_vex(glm::vec3  ver[], int vertexnum)
//	{
//		delete _vertices;
//		_vextexnum = vertexnum;
//		_vertices = new glm::vec3[vertexnum];
//		for(int i = 0;i<vertexnum;i++)
//		{
//			_vertices[i] = ver[i];
//		}
//	}
//	void set_part(globject_part part[], int partnum)
//	{
//		delete _part;
//		_partnum = partnum;
//		
//		_part = new globject_part[partnum];
//
//		for(int i = 0; i<partnum; i++)
//		{
//			_part[i] = part[i];
//		}
//	}
//	void set_color(glm::vec3 color[], int colornum)
//	{
//		for(int i = 0; i<colornum; i++)
//		{
//			_part->_color = color[i];
//		}
//	}
//
//	void changecolor(glm::vec3 const & color, int partindex)
//	{
//		_part[partindex]._color = color;
//	}
//	void set_indices(int ** indices,int indices_len[], int partnum)
//	{
//		for(int i = 0; i<partnum; i++)
//		{
//			_part[i].assign_index(*(indices+i),indices_len[i]);
//		}
//	}
//
//	void loadbuffer(int partid, GLuint vertices_state = GL_STATIC_DRAW)
//	{
//		int verticesnum = _part[partid]._indices_num;
//		int k = -1 ;
//		float * vertices = new float[verticesnum * 6];
//
//		for(int i = 0;i<verticesnum; i++)
//		{
//			vertices[++k] = _vertices[_part[partid]._vex_indices[i]].x;
//			vertices[++k] = _vertices[_part[partid]._vex_indices[i]].y;
//			vertices[++k] = _vertices[_part[partid]._vex_indices[i]].z;
//			vertices[++k] = _part[partid]._color.x;
//			vertices[++k] = _part[partid]._color.y;
//			vertices[++k] = _part[partid]._color.z;
//		}
//		_paintobject[partid].loadvertex(vertices, verticesnum * 6 * sizeof(float),vertices_state);
//	}
//	void setbuffer(GLuint vertices_state = GL_STATIC_DRAW)
//	{
//		for(int i=0; i<_partnum; i++)
//		{
//			loadbuffer(i, vertices_state);
//		}
//	}
//
//	void drawobject_part(int partid, GLenum mode = GL_TRIANGLES)
//	{
//		_paintobject[partid].glPaintElements(mode, 0, _part[partid]._indices_num);
//	}
//	void draw(GLenum mode = GL_TRIANGLES)
//	{
//		for(int i = 0;i<_partnum;i++)
//		{
//			_paintobject[i].glPaintElements(mode, 0, _part[i]._indices_num);
//		}
//	}
};
#endif