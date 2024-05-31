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
#include "Distribution.h"

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

const float radius = 0.01;
const float sectorCount = 30; // Min 3
const float stackCount = 30; // min 2

Distribution distribution(0.0, 0.01, 10);

std::vector<float> posX, posY;
std::vector<std::pair<float, float>> pos;
#define N_SPHERES	(uint32_t) 1000
#define SAMPLES	(uint32_t) 1000

#define VS 6

void createSpherePoints()
{
	sphereVertices.push_back(0);
	sphereVertices.push_back(0);
	sphereVertices.push_back(-radius);

	sphereVertices.push_back(0.0);
	sphereVertices.push_back(0.0);
	sphereVertices.push_back(0.0);
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

			sphereVertices.push_back(0.0);
			sphereVertices.push_back(0.0);
			sphereVertices.push_back(0.0);
		}
	}
	
	sphereVertices.push_back(0);
	sphereVertices.push_back(0);
	sphereVertices.push_back(+radius);

	sphereVertices.push_back(0.0);
	sphereVertices.push_back(0.0);
	sphereVertices.push_back(0.0);
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
		sphereIndices.push_back(i + sectorCount);
		sphereIndices.push_back(i + 1);
		

		sphereIndices.push_back(i + 1);
		sphereIndices.push_back(i + sectorCount);
		sphereIndices.push_back(i + sectorCount + 1);
	}
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

//void createSphere() {
//	createSpherePoints();
//	createSphereIndices();
//
//	calcAverageNormals(&sphereIndices[0], sphereIndices.size(), &sphereVertices[0], sphereVertices.size(), VS, 3);
//
//	Mesh* obj1 = new Mesh();
//	obj1->CreateMesh(&sphereVertices[0], &sphereIndices[0], sphereVertices.size(), sphereIndices.size());
//	meshList.push_back(obj1);
//}


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
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void createSphere() {
	createSpherePoints();
	createSphereIndices();

	calcAverageNormals(&sphereIndices[0], sphereIndices.size(), &sphereVertices[0], sphereVertices.size(), VS, 3);

	for (size_t i = 0; i < 2 * N_SPHERES; i++)
	{
		meshList.push_back(new Mesh());
		meshList[i]->CreateMesh(&sphereVertices[0], &sphereIndices[0], sphereVertices.size(), sphereIndices.size());
	}
}

float reverse_BS(float x, std::vector<float> v)
{
	int l = 0, h = v.size() - 1, mid = (l + h) / 2;
	assert(x <= 1.0 && x >= 0.0, "x out of legit range !");
	while (h - l != 1)
	{
		if (x >= v[mid])
		{
			l = mid;
		}
		else h = mid;
		mid = (l + h) / 2;
	}
	auto width = distribution.getWidth();
	auto std = distribution.getStd();
	auto mean = distribution.getMean();
	return (- width * std / 2 + h * (width * std) / SAMPLES) + mean;
}

void compPos(std::vector<std::pair<float, float>>& pos)
{
	std::srand(std::time(nullptr));
	auto cdt = distribution.getCDT(SAMPLES);
	for (size_t i = 0; i < N_SPHERES; i++)
	{
		float x_ = (float)rand() / RAND_MAX;
		float y_ = (float)rand() / RAND_MAX;
		pos.push_back({ reverse_BS(x_, cdt), reverse_BS(y_, cdt) });
	}
}

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();

	/*compPos(posX);
	compPos(posY);*/
	compPos(pos);
	createSphere();
	

	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);
	light = Light(0.0, 1.0, 0.0, 0.2, -0.5, -0.5, -0.5, 0.6);
	material = Material(0.9, 128);


	GLuint uniformProjectionLocation = 0, uniformModelLocation = 0, uniformViewLocation = 0, ambienIntensityLocation = 0,
		ambientColourLocation = 0, diffuseIntensityLocation = 0, lightDirectionLocation = 0, specularIntensityLocation = 0, shininessLocation = 0;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		//printf("Running while ...\n");
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
		uniformModelLocation = shaderList[0].GetModelLocation();
		uniformProjectionLocation = shaderList[0].GetProjectionLocation();
		uniformViewLocation = shaderList[0].GetViewLocation();
		ambienIntensityLocation = shaderList[0].GetAmbienIntensityLocation();
		ambientColourLocation = shaderList[0].GetAmbientColourLocation();
		diffuseIntensityLocation = shaderList[0].GetDiffuseIntensityLocation();
		lightDirectionLocation = shaderList[0].GetDirectionLocation();
		specularIntensityLocation = shaderList[0].GetSpecularIntensityLocation();
		shininessLocation = shaderList[0].GetShininessLocation();


		light.UseLight(ambienIntensityLocation, ambientColourLocation, diffuseIntensityLocation, lightDirectionLocation);
		material.UseMaterial(specularIntensityLocation, shininessLocation);

		glm::mat4 model(1.0f);

		for (size_t i = 0; i < N_SPHERES; i++){
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(pos[i].first, pos[i].second, -2.5f));
			glUniformMatrix4fv(uniformModelLocation, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(uniformProjectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformViewLocation, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			meshList[i]->RenderMesh();
		}

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}