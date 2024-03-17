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

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

std::vector<float> sphereVertices;
std::vector<size_t> sphereIndices;

const float radius = 0.5;
const float sectorCount = 4;
const float stackCount = 2;

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
			//printf("test\n");
#ifdef _DEBUG
			printf("%f %f\n", phi, theta);
#endif

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
	for (int i = 0; i < sphereVertices.size() - sectorCount; i++)
	{
		if (i == 0 || i == sphereIndices.size())
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
		else {
			sphereIndices.push_back(i);
			sphereIndices.push_back(i+1);
			sphereIndices.push_back(i+sectorCount);

			sphereIndices.push_back(i + 1);
			sphereIndices.push_back(i + sectorCount);
			sphereIndices.push_back(i + sectorCount + 1);
		}
	}
}

void createSphere() {
	//createSpherePoi/*nts();
	//createSphereIndices();*/


	unsigned int indices[] = {
		0,1,2,
		0,2,3,
		0,1,3,
		4,1,2,
		4,1,3,
		4,2,3
	};

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12,12); 
	meshList.push_back(obj1);
}

void printVertices()
{
	for (size_t i = 0; i < sphereVertices.size(); i++)
	{
		std::cout << sphereVertices[i] << ",";
	}
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

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, sizeof(vertices)/sizeof(vertices[0]), sizeof(indices)/sizeof(indices[0]));
	meshList.push_back(obj1);

	/*Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj2);*/
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

	CreateObjects();

	createSpherePoints();
	printVertices();

	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

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
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[0]->RenderMesh();

		//model = glm::mat/*4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//meshList[1]->RenderMesh();*/

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}