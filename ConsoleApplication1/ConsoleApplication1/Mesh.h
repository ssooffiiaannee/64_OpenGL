#pragma once

#include <GL\glew.h>
#include <vector>

class Mesh
{
public:
	Mesh();

	void CreateMesh(float vertices[], unsigned int indices[], unsigned int numOfVertices, unsigned int numOfIndices);
	void RenderMesh();
	void ClearMesh();

	~Mesh();

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
};

