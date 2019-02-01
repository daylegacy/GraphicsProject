#pragma once
#include <glew.h>
#include <glfw3.h>
#include<vector>
#include "Vertex.h"

class Primitive {
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
public:
	Primitive() {}
	virtual ~Primitive()
	{}
	void set(const Vertex * vertices, const unsigned nrOfVertices,
		const GLuint * indices, const unsigned nrOfIndices) {
		for (size_t i = 0; i <nrOfVertices; i++)
		{
			this->vertices.push_back(vertices[i]);
		}
		for (size_t i = 0; i < nrOfIndices; i++)
		{
			this->indices.push_back(indices[i]);
		}
	}
	inline Vertex* getVertices() {return vertices.data();}
	inline GLuint* getIndices() { return indices.data(); }

	inline const unsigned getNrOfVertices() { return vertices.size(); }
	inline const unsigned getNrOfIndices() { return indices.size(); }


};

class Quad :public Primitive {
public:
	Quad()
	: Primitive()
	{
		Vertex vertices[] = {
			//Position							//color						//texcoords				//normals
			glm::vec3(-0.5f, 0.5f, 0.f),	glm::vec3(1.f, 0.f, 0.f),	glm::vec2(0.f, 2.f),	glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(-0.5f, -0.5f, 0.f),	glm::vec3(0.f, 1.f, 0.f),	glm::vec2(0.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f,-0.5f, 0.f),		glm::vec3(0.f, 0.f, 1.f),	glm::vec2(2.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, 0.5f, 0.f),		glm::vec3(1.f, 1.f, 0.f),	glm::vec2(2.f, 2.f),	glm::vec3(0.f, 0.f, 1.f)
		};
		unsigned nrOfVertices = sizeof(vertices) / sizeof Vertex;

		GLuint indices[] =
		{
			0, 1, 2, //triangle 1
			0, 2, 3 //tringle 2
		};
		unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);
		this->set(vertices, nrOfVertices, indices, nrOfIndices);
	}
};

class Triangle :public Primitive {
public:
	Triangle()
		: Primitive()
	{
		Vertex vertices[] = {
			//Position							//color						//texcoords				//normals
			glm::vec3(-0.5f, 0.5f, 0.f),	glm::vec3(1.f, 0.f, 0.f),	glm::vec2(0.f, 1.f),	glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(-0.5f, -0.5f, 0.f),	glm::vec3(0.f, 1.f, 0.f),	glm::vec2(0.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f,-0.5f, 0.f),		glm::vec3(0.f, 0.f, 1.f),	glm::vec2(1.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),
		};
		unsigned nrOfVertices = sizeof(vertices) / sizeof Vertex;

		GLuint indices[] =
		{
			0, 1, 2, //triangle 1
		};
		unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);
		this->set(vertices, nrOfVertices, indices, nrOfIndices);
	}
};