#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;
Light light;
Material material;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

std::vector<float> sphereVertices;
std::vector<unsigned int> sphereIndices;

#define VS 3

const float radius = 2;
const float sectorCount = 50; // Min 3
const float stackCount = 50; // min 2

void createSpherePoints()
{
	sphereVertices.push_back(0);
	sphereVertices.push_back(0);
	sphereVertices.push_back(-radius);
	for (std::size_t i = 1; i < stackCount; i++)
	{
		for (std::size_t j = 0; j < sectorCount; j++)
		{
			float phi = (glm::pi<float>() * (float) i) / (float)stackCount - glm::pi<float>()/2;
			float theta = (2 * glm::pi<float>() * (float) j) / (float)sectorCount;

			float y = radius * glm::cos(phi) * glm::sin(theta);
			float x = radius * glm::cos(phi) * glm::cos(theta);
			float z = radius * glm::sin(phi);

			sphereVertices.push_back(x);
			sphereVertices.push_back(y);
			sphereVertices.push_back(z);
		}
	}
	
	sphereVertices.push_back(0);
	sphereVertices.push_back(0);
	sphereVertices.push_back(+radius);
}

void createSphereIndices()
{
	size_t n_points = sphereVertices.size() / VS;
	for (auto i : { (size_t) 0, n_points - 1 })
	{
		int d = (i == 0) ? 1 : -1;
		for (size_t j = 1; j < sectorCount + 1; j++)
		{
			sphereIndices.push_back(i);
			sphereIndices.push_back(i + d * j);
			if (j != sectorCount)
				sphereIndices.push_back(i + d * (j + 1));
			else
				sphereIndices.push_back(i + d * 1);
		}
	}
	for (int i = 1; i < n_points - sectorCount - 2; i++)
	{
		sphereIndices.push_back(i);
		sphereIndices.push_back(i + 1);
		sphereIndices.push_back(i + sectorCount);

		sphereIndices.push_back(i + 1);
		sphereIndices.push_back(i + sectorCount);
		sphereIndices.push_back(i + sectorCount + 1);
	}
}

void createSphere() {
	createSpherePoints();
	createSphereIndices();

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(&sphereVertices[0], &sphereIndices[0], sphereVertices.size(), sphereIndices.size());
	meshList.push_back(obj1);
}

void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void printVerticesIndices()
{
	std::cout << "Indices size : " << sphereIndices.size() << std::endl;
	std::cout << "Vertices size: " << sphereVertices.size() << std::endl;

	for (size_t i = 0; i < sphereVertices.size(); i++)
	{
		std::cout << sphereVertices[i] << " ,";
	}
	std::cout << std::endl;
	for (size_t i = 0; i < sphereIndices.size(); i++)
	{
		std::cout << sphereIndices[i] << " ,";
	}
	std::cout << std::endl;
}

void CreateObjects()
{
	unsigned int indices[] = {
		0,1,2,
		0,2,3,
		0,3,4,
		0,4,1,
		5,1,2,
		5,2,3,
		5,3,4,
		5,4,1
	};

	GLfloat vertices[] = {
		0, 0, -0.5,
		0.5, 0, 0,
		-2.18557e-08, 0.5, 0,
		-0.5, -4.37114e-08, 0,
		5.96244e-09, -0.5, 0,
		0, 0, 0.5
	};

	/*Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, sizeof(vertices)/sizeof(vertices[0]), sizeof(indices)/sizeof(indices[0]));
	meshList.push_back(obj1);*/
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();

	//CreateObjects();
	createSphere();
#ifdef _DEBUG
	printVerticesIndices();
#endif
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);
	light = Light(1.0, 0.0, 0.0, 0.2, -0.5, -0.5, -0.5, 0.2);
	material = Material(0.2, 0.2);

	light.UseLight();
	material.UseMaterial();


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTime = now;

		// Get + Handle User Input
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();

		glm::mat4 model(1.0f);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[0]->RenderMesh();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}