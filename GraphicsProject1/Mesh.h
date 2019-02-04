#pragma once
#include<iostream>
#include<vector>

#include "shader.h"
#include "texture.h"
#include "Material.h"
#include "Vertex.h"
#include "Primitives.h"

class Mesh
{
private:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	unsigned int nrOfVertices;
	unsigned int nrOfIndices;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scaling;
	glm::mat4 ModelMatrix;

	void initVAO(Primitive * primitive) {
		this->nrOfVertices = primitive->getNrOfVertices();
		this->nrOfIndices = primitive->getNrOfIndices();

		//create VAO
		glCreateVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		//GEN VBO AND BIND AND SEND DATA
		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices*sizeof(Vertex), primitive->getVertices(), GL_STATIC_DRAW);

		//GEN EBO AND BIND AND SEND DATA
		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices * sizeof(GLuint), primitive->getIndices(), GL_STATIC_DRAW);

		//SET VERTEXATTRIBPOINTERS AND ENABLE (INPUT ASSEMBLY)
		//GLuint attribLoc = glGetAttribLocation(core_program, "vertex_position");
		//Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);
		//Color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);
		//Texcoord
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
		glEnableVertexAttribArray(2);

		//normal
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);

		//bind VAO 0
		glBindVertexArray(0);
	}

	void initVAO(Vertex* vertexArray,
		const unsigned& nrOfVertices,
		GLuint* indexArray,
		const unsigned& nrOfIndices
	) {
		this->nrOfVertices = nrOfVertices;
		this->nrOfIndices = nrOfIndices;

		//create VAO
		glCreateVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		//GEN VBO AND BIND AND SEND DATA
		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), vertexArray, GL_STATIC_DRAW);

		//GEN EBO AND BIND AND SEND DATA
		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices * sizeof(GLuint), indexArray, GL_STATIC_DRAW);

		//SET VERTEXATTRIBPOINTERS AND ENABLE (INPUT ASSEMBLY)
		//GLuint attribLoc = glGetAttribLocation(core_program, "vertex_position");
		//Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);
		//Color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);
		//Texcoord
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
		glEnableVertexAttribArray(2);

		//normal
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);

		//bind VAO 0
		glBindVertexArray(0);
	}

	void updateUniforms(Shader * shader) {
		shader->setMat4fv(this->ModelMatrix, "ModelMatrix");
	}



public:
	Mesh(
		Vertex* vertexArray,
		const unsigned& nrOfVertices,
		GLuint* indexArray,
		const unsigned& nrOfIndices,
		glm::vec3 position = glm::vec3(0.f),
		glm::vec3 rotation = glm::vec3(0.f),
		glm::vec3 scale = glm::vec3(1.f)
	) 
	{
		this->position = position;
		this->rotation = rotation;
		this->scaling = scale;

		this->initVAO(vertexArray, nrOfVertices, indexArray, nrOfIndices);
		
		this->updateModelMatrix();
	}

	Mesh(
		Primitive * primitive, 
		glm::vec3 position = glm::vec3(0.f),
		glm::vec3 rotation = glm::vec3(0.f),
		glm::vec3 scale = glm::vec3(1.f)
	)
	{
		this->position = position;
		this->rotation = rotation;
		this->scaling = scale;

		this->initVAO(primitive);

		this->updateModelMatrix();
	}
	~Mesh(){
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);
		glDeleteBuffers(1, &this->EBO);
	}

	//accessors

	//modifieres
	void setPosition(const glm::vec3 position) {
		this->position = position;
	}
	void setRotation(const glm::vec3 rotation) {
		this->rotation = rotation;
	}
	void setScale(const glm::vec3 scale) {
		this->scaling = scale;
	}

	
	//functions
	void move(const glm::vec3 position) {
		this->position += position;
	}

	void rotate(const glm::vec3 rotation) {
		this->rotation+= rotation;
	}

	void scale(const glm::vec3 scale) {
		this->scaling += scale;
	}

	void updateModelMatrix() {
		this->ModelMatrix = glm::mat4(1.f);
		ModelMatrix = glm::translate(ModelMatrix, this->position);
		ModelMatrix = glm::rotate(ModelMatrix, this->rotation.x, glm::vec3(1.f, 0.f, 0.f));
		ModelMatrix = glm::rotate(ModelMatrix, this->rotation.y, glm::vec3(0.f, 1.f, 0.f));
		ModelMatrix = glm::rotate(ModelMatrix, this->rotation.z, glm::vec3(0.f, 0.f, 1.f));
		ModelMatrix = glm::scale(ModelMatrix, this->scaling);
	}
	void render(Shader * shader) {
		//update uniforms
		this->updateModelMatrix();
		this->updateUniforms(shader);
		shader->use();
		glBindVertexArray(this->VAO);
		/*if (!nrOfIndices) {
			glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);
		}
		else {*/
			glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);
		//}
		shader->unuse();

	}
};

