#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Resources/Shaders/shaderClass.h"
#include "Resources/Shaders/VAO.h"
#include "Resources/Shaders/VBO.h"
#include "Resources/Shaders/EBO.h"
#include "Camera.h"


const unsigned int width = 800;
const unsigned int height = 800;
using namespace std;
struct Vertex {
	float x, y, z;
	float r, g, b;
	float pr, pg, pb;
};
class Cube {
public:
	vector<Vertex> mVertecies;
	glm::vec3 position;
	float scaleX, scaleY, scaleZ;
	std::string name;
	glm::vec3 velocity;

	Cube(float scale, const glm::vec3& initialPosition = glm::vec3(0.0f,0.0f,0.0f), float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f)
		: a(scale), position(initialPosition), scaleX(scaleX), scaleY(scaleY), scaleZ(scaleZ), velocity(glm::vec3(0.0f)) {
		Vertex v0{ -a, -a, a , 1.0f, 0.0f, 0.0f };
		Vertex v1{ a, -a, a , 0.0f, 1.0f, 0.0f };
		Vertex v2{ a, a, a , 0.0f, 0.0f, 1.0f };
		Vertex v3{ -a, a, a , 0.0f, 1.0f, 0.0f };
		Vertex v4{ -a, -a, -a , 1.0f, 1.0f, 0.0f };
		Vertex v5{ a, -a, -a , 0.0f, 1.0f, 0.0f };
		Vertex v6{ a, a, -a , 0.0f, 1.0f, 1.0f };
		Vertex v7{ -a, a, -a , 0.0f, 1.0f, 0.0f };


		// Front face

		mVertecies.push_back(v0);
		mVertecies.push_back(v1);
		mVertecies.push_back(v3);
		mVertecies.push_back(v3);
		mVertecies.push_back(v1);
		mVertecies.push_back(v2);



		// Back face
		mVertecies.push_back(v4);
		mVertecies.push_back(v5);
		mVertecies.push_back(v7);
		mVertecies.push_back(v7);
		mVertecies.push_back(v5);
		mVertecies.push_back(v6);

		// Right face
		mVertecies.push_back(v1);
		mVertecies.push_back(v5);
		mVertecies.push_back(v2);
		mVertecies.push_back(v2);
		mVertecies.push_back(v5);
		mVertecies.push_back(v6);

		// Left face
		mVertecies.push_back(v4);
		mVertecies.push_back(v0);
		mVertecies.push_back(v7);
		mVertecies.push_back(v7);
		mVertecies.push_back(v0);
		mVertecies.push_back(v3);

		// Top face
		mVertecies.push_back(v3);
		mVertecies.push_back(v2);
		mVertecies.push_back(v7);
		mVertecies.push_back(v7);
		mVertecies.push_back(v2);
		mVertecies.push_back(v6);

		// Bottom face
		mVertecies.push_back(v4);
		mVertecies.push_back(v5);
		mVertecies.push_back(v0);
		mVertecies.push_back(v0);
		mVertecies.push_back(v5);
		mVertecies.push_back(v1);

		
		
		flattenVertices();
	}
	std::vector<GLfloat> getFlattenedVertices() const {
		std::vector<GLfloat> flattenedVertices;
		for (const Vertex& vertex : mVertecies) {
			flattenedVertices.push_back((vertex.x * scaleX) + position.x);
			flattenedVertices.push_back((vertex.y * scaleY) + position.y);
			flattenedVertices.push_back((vertex.z * scaleZ) + position.z);
			flattenedVertices.push_back(vertex.r);
			flattenedVertices.push_back(vertex.g);
			flattenedVertices.push_back(vertex.b);
		}
		return flattenedVertices;
	}
	void UpdateVertices(float Xspeed,float Yspeed,float Zspeed) {

		for (Vertex& vertex : mVertecies) {
			vertex.x +=  Xspeed;
			vertex.y +=  Yspeed;
			vertex.z +=  Zspeed;
		}
		
	}
	void UpdatePosition(float deltaTime) {
		position += velocity * deltaTime;
	}
	bool CheckCollision(const Cube& otherCube) const {
		// Iterate through each vertex in the current cube
		for (const Vertex& vertex : mVertecies) {
			// Iterate through each vertex in the other cube
			for (const Vertex& otherVertex : otherCube.mVertecies) {
				// Calculate the distance between the two vertices
				float distance = glm::length(glm::vec3(vertex.x * scaleX + position.x,
					vertex.y * scaleY + position.y,
					vertex.z * scaleZ + position.z) -
					glm::vec3(otherVertex.x * otherCube.scaleX + otherCube.position.x,
						otherVertex.y * otherCube.scaleY + otherCube.position.y,
						otherVertex.z * otherCube.scaleZ + otherCube.position.z));

				// Check if the distance is less than a threshold (adjust as needed)
				if (distance < 0.5f) {
					// Collision detected
					return true;
				}

			}
		}

		// No collision detected
		return false;
	}
	float GetA() {

		return a;
	}
private:
	void flattenVertices() {
		std::vector<GLfloat> flattenedVertices;
		for (const Vertex& vertex : mVertecies) {
			flattenedVertices.push_back((vertex.x * scaleX) + position.x);
			flattenedVertices.push_back((vertex.y * scaleY) + position.y);
			flattenedVertices.push_back((vertex.z * scaleZ) + position.z);
			flattenedVertices.push_back(vertex.r);
			flattenedVertices.push_back(vertex.g);
			flattenedVertices.push_back(vertex.b);
		}
	}
	float a{ 1.0f };
};


struct Point {
	float x, y, z;

};

void CreateCoordinateSystem(std::vector<Vertex>& vertices) {
	// X-axis
	vertices.push_back({ -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f });
	vertices.push_back({ 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f });

	// Y-axis
	vertices.push_back({ 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f });
	vertices.push_back({ 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f });

	// Z-axis
	vertices.push_back({ 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f });
	vertices.push_back({ 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f });
}
void writeToFile(const char* fileName, double x, double y, double z, double r, double g, double b) {
	std::ofstream outputFile(fileName, std::ios::app);  // Open the file in append mode
	if (outputFile.is_open()) {
		outputFile << std::fixed << std::setprecision(3);
		outputFile << x << " , " << y << " , " << z << " , " << r << " , " << g << " , " << b;


	}
	else {
		std::cerr << "Unable to open the output file for writing." << std::endl;
	}
	//if (y > 0) {
	//	outputFile << " green";  // Use green for positive y
	//}
	//else if (y < 0) {
	//	outputFile << " red";    // Use red for negative y
	//}
	//else {
	//	outputFile << " white";  // Use white for y = 0
	//}
	outputFile << std::endl;
	outputFile.close();
}
void Readfile(const char* fileName, std::vector<Vertex>& verticesSpiral) {
	std::ifstream inputFile(fileName);
	if (inputFile.is_open()) {


		std::string line;
		std::getline(inputFile, line);
		Vertex vertex;
		char comma; // to capture the commas in the file

		while (inputFile >> vertex.x >> comma >> vertex.y >> comma >> vertex.z >> comma 
			>> vertex.r >> comma >> vertex.g >> comma >> vertex.b) {
			verticesSpiral.push_back(vertex);
		}

		inputFile.close();
	}
	else {
		std::cerr << "Unable to open the input file for reading." << std::endl;
	} 
	
	
	
}
glm::vec4 CubicHermiteFunction(float inx0, float inx1, float iny0,float iny1, float derivativeY0, float derivativeY1) {
	float x0, x1, y0, y1;
	x0 = inx0;
	x1 = inx1;
	y0 = iny0;
	y1 = iny1;

	float derY0, derY1;
	derY0 = derivativeY0;
	derY1 = derivativeY1;

	float func = powf(1, 1);
	glm::mat<4, 4, float> A
	    (powf(x0, 3), powf(x0, 2), x0, 1,
		powf(x1, 3), powf(x1, 2), x1, 1,
		powf(x0, 2), x0, 1, 0,
		powf(x0, 2), x0, 1, 1);
	
	glm::vec4 y(y0, y1,derivativeY0,derivativeY1);

	glm::mat4 inverseA = glm::inverse(A);

	glm::vec4 x = y * inverseA;
	return x;
}
glm::vec3 LeastSquareMethod() {
#define G 5
	glm::mat3x3 ATA(279, -5, -1,   // First column
		-5, 368, 48,  // Second column
		-1, 48, 8);
	              
	
	
	
	
	//compute A^T
	
 // amount of points

	//compute A^T * y
	glm::vec3 ATy(-5,368,48);

	//compute B^-1 or (A * A^T)^-1
	glm::mat3 ATAInverse = glm::inverse(ATA);

	
	glm::vec3 x = ATAInverse * ATy;
	/*std::cout << "ATy: " << ATy.x << ", " << ATy.y << ", " << ATy.z << std::endl;
    std::cout << "ATAInverse: " << ATAInverse[0][0] << ", " << ATAInverse[0][1] << ", " << ATAInverse[0][2] << std::endl;
    std::cout << "            " << ATAInverse[1][0] << ", " << ATAInverse[1][1] << ", " << ATAInverse[1][2] << std::endl;
    std::cout << "            " << ATAInverse[2][0] << ", " << ATAInverse[2][1] << ", " << ATAInverse[2][2] << std::endl;*/

    // compute x
	
	std::cout << " x: " << x.x << " y: " << x.y << " z: " << x.z << std::endl;
	return x;

}
glm::vec3 CalculateMTV(const Cube& cube1, const Cube& cube2)
{
	// Calculate the vector from cube1 to cube2
	glm::vec3 vectorFrom1to2 = cube2.position - cube1.position;

	// Calculate the minimum translation vector (MTV)
	float xOverlap = fabs(vectorFrom1to2.x) - (cube1.scaleX + cube2.scaleX) / 2.0f;
	float zOverlap = fabs(vectorFrom1to2.z) - (cube1.scaleZ + cube2.scaleZ) / 2.0f;

	glm::vec3 MTV(0.0f);

	if (xOverlap < zOverlap)
	{
		MTV.x = (vectorFrom1to2.x > 0) ? xOverlap : -xOverlap;
	}
	else
	{
		MTV.z = (vectorFrom1to2.z > 0) ? zOverlap : -zOverlap;
	}

	return MTV;
}
void CreateGraphFromFunction(std::vector<Vertex>& verticesgraph,float c , int iterations, const char* filename, int start) {
	for (int i = start; i <= iterations; ++i) {
		float t = static_cast<float>(i);
		float n = 0.05f;
		float x = i * n;
		float y = LeastSquareMethod().x * x * x + LeastSquareMethod().y * x + LeastSquareMethod().z;
		float z = 0.0f;
	
		float df = 2*x;
		

		Vertex vertex;
		vertex.x = x;
		vertex.y = y;
		vertex.z = z;
		if (df < 0) {
			vertex.r = 1.0f;
			vertex.g = 0.0f;
		}
		else {
			vertex.r = 0.0f;
			vertex.g = 1.0f;
		}
		
		vertex.b = std::abs(z) / (c * iterations);  // Adjust for coloring effect
		/*vertex.pr = 0.0f;
		vertex.pg = 0.0f;
		vertex.pb = 1.0f;*/
		verticesgraph.push_back(vertex);
		writeToFile(filename, vertex.x, vertex.y, vertex.z,vertex.r, vertex.g, vertex.b);

	}

	

}

int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "OpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGLProject", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	
	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	
	
	const char* outputFileGraphTwoVar = "grahTwoVardata.txt";
	

	
	glViewport(0, 0, width, height);

	std::vector<Vertex> verticesGraph;
	//Readfile("grahTwoVardata.txt", verticesGraph);
	
	
	int iterations = 200;
	int start = -200;

	const int numRows = 3;
	const int numCols = 4;

	glm::mat<3, 4, float> A(-2, 2, 1,   
		-5, 4, 1,  
		-7, 7, 1,
		-8, 11, 1
	    );  

	glm::mat<3, 4, float> B(4, 2, 1,
		-2, -1, 1,
		1, 1, 1,
		0, 0, 0
	);

	std::vector<glm::vec3> points;
	for (int i = 0; i < numCols; ++i) {
		glm::vec3 point = glm::vec3(A[0][i], A[1][i], A[2][i]);
		points.push_back(point);
	}
	std::vector<GLfloat> flattenedPoints;
	for (const glm::vec3& point : points) {
		flattenedPoints.push_back(point.x);
		flattenedPoints.push_back(point.y);
		flattenedPoints.push_back(point.z);
		// Add color components if needed
	}
	
	

	
	//FunctionWithTwoVariables(verticesGraph, iterations, outputFileGraphTwoVar);
	//CreateGraphFromFunction(verticesGraph, c, iterations, outputFileGraphTwoVar, start);
	//CreateCoordinateSystem(coordinateSystemVertices);
	// Generates Shader object using shaders defualt.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");
	
	
	
	

	// Flatten the vector of Vertex into GLfloat
	

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();
	std::vector<GLfloat> flattenedVertices;
	for (const Vertex& vertex : verticesGraph) {
		flattenedVertices.push_back(vertex.x);
		flattenedVertices.push_back(vertex.y);
		flattenedVertices.push_back(vertex.z);
		flattenedVertices.push_back(vertex.r);
		flattenedVertices.push_back(vertex.g);
		flattenedVertices.push_back(vertex.b);
	}
	

	// Unbind to prevent accidentally modifying it
	

	
	Cube mahCube(1.5f,glm::vec3(0.0f,-10.0f,0.0f),10.0f,0.5f,10.0f);
	std::vector<GLfloat> flattenedCubeVertices = mahCube.getFlattenedVertices();

	Cube Cube2(1.0f, glm::vec3(0.0f, -8.0f, 0.0f), 1.0f, 1.0f, 1.0f);
	std::vector<GLfloat> flattenedCube2Vertices = Cube2.getFlattenedVertices();

	Cube Cube3(1.0f, glm::vec3(5.0f, -8.0f, 0.0f), 1.0f, 1.0f, 1.0f);
	std::vector<GLfloat> flattenedCube3Vertices = Cube3.getFlattenedVertices();



	// Generates Vertex Buffer Object and links it to cube
	//should probably have this in the class but nah, not yet
	VBO VBO_cube(flattenedCubeVertices.data(), flattenedCubeVertices.size() * sizeof(GLfloat));
	VAO1.LinkAttrib(VBO_cube, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO_cube, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	VAO VAO2;
	VAO2.Bind();
	VBO VBO_Cube2(flattenedCube2Vertices.data(), flattenedCube2Vertices.size() * sizeof(GLfloat));
	VAO2.LinkAttrib(VBO_Cube2, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO2.LinkAttrib(VBO_Cube2, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	
	
	VAO VAO3;
	VAO3.Bind();
	VBO VBO_Cube3(flattenedCube3Vertices.data(), flattenedCube3Vertices.size() * sizeof(GLfloat));
	VAO3.LinkAttrib(VBO_Cube3, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO3.LinkAttrib(VBO_Cube3, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	
	VAO1.Unbind();
	VBO_cube.Unbind();
	VAO2.Unbind();
	VBO_Cube2.Unbind();
	VAO3.Unbind();
	VBO_Cube3.Unbind();

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
	float scaleValue = 100.0f;
	
	// Variables that help the rotation of the pyramid
	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	
	double prevFrameTime = 0.0f;
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
	// Main while loop
	float translationSpeed = 0.0005f;
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		
		camera.Inputs(window);
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");


		glUniform1f(uniID, scaleValue);
		 //Bind the VAO so OpenGL knows to use it
	
		//draw first cube (floor)
		VAO1.Bind();
		glDrawArrays(GL_TRIANGLES, 0, mahCube.mVertecies.size());
		VAO1.Unbind();

		//draw second cube
		VAO2.Bind();
		glDrawArrays(GL_TRIANGLES, 0, Cube2.mVertecies.size());
		VAO2.Unbind();
		double currentFrameTime = glfwGetTime();
		double deltaTime = currentFrameTime - prevFrameTime;
		prevFrameTime = currentFrameTime;
		
		VAO3.Bind();
		glDrawArrays(GL_TRIANGLES, 0, Cube3.mVertecies.size());
		VAO3.Unbind();
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			float originalPosition = Cube2.position.x;

			// Check if the new position will cause a collision
			if (!Cube2.CheckCollision(Cube3))
			{
				Cube2.position.x = originalPosition - translationSpeed;

				// Update vertices and VBO
				Cube2.UpdateVertices(-0.05f, 0.0f, 0.0f);
				std::vector<GLfloat> flattenedCube2Vertices = Cube2.getFlattenedVertices();
				VBO_Cube2.UpdateData(flattenedCube2Vertices.data(), flattenedCube2Vertices.size() * sizeof(GLfloat));
			}
			else
			{
				// Handle collision response (adjust position)
				// You can use MTV or collision point information here
				Cube2.position.x -= CalculateMTV(Cube2, Cube3).x; /* Adjusted position based on collision information */;
				printf(" Collided ");
				// Update vertices and VBO
				
				Cube2.UpdateVertices(-0.08, 0.0f, 0.0f);
				std::vector<GLfloat> flattenedCube2Vertices = Cube2.getFlattenedVertices();
				VBO_Cube2.UpdateData(flattenedCube2Vertices.data(), flattenedCube2Vertices.size() * sizeof(GLfloat));
			}
		}

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			float newPosition = Cube2.position.x + translationSpeed;

			// Check if the new position will cause a collision
			if (!Cube2.CheckCollision(Cube3))
			{
				Cube2.position.x = newPosition;

				// Update vertices and VBO
				Cube2.UpdateVertices(0.05f, 0.0f, 0.0f);
				std::vector<GLfloat> flattenedCube2Vertices = Cube2.getFlattenedVertices();
				VBO_Cube2.UpdateData(flattenedCube2Vertices.data(), flattenedCube2Vertices.size() * sizeof(GLfloat));
			}
			else
			{
				// Calculate minimum translation vector (MTV) to separate cubes
				glm::vec3 MTV = CalculateMTV(Cube2, Cube3);
				printf(" Collided ");
				// Move the cubes away from each other using the MTV
				Cube2.position.x += MTV.x;
				Cube2.UpdateVertices(0.00f, 0.0f, 0.0f);
				std::vector<GLfloat> flattenedCube2Vertices = Cube2.getFlattenedVertices();
				VBO_Cube2.UpdateData(flattenedCube2Vertices.data(), flattenedCube2Vertices.size() * sizeof(GLfloat));
			}
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			float newPosition = Cube2.position.z - translationSpeed;

			// Check if the new position will cause a collision
			if (!Cube2.CheckCollision(Cube3))
			{
				Cube2.position.z = newPosition;

				// Update vertices and VBO
				Cube2.UpdateVertices(0.0f, 0.0f, -0.05f);
				std::vector<GLfloat> flattenedCube2Vertices = Cube2.getFlattenedVertices();
				VBO_Cube2.UpdateData(flattenedCube2Vertices.data(), flattenedCube2Vertices.size() * sizeof(GLfloat));
			}
			else
			{
				// Calculate minimum translation vector (MTV) to separate cubes
				glm::vec3 MTV = CalculateMTV(Cube2, Cube3);

				// Move the cubes away from each other using the MTV
				Cube2.position.z -= MTV.z;
				Cube2.UpdateVertices(0.00f, 0.0f, 0.0f);
				std::vector<GLfloat> flattenedCube2Vertices = Cube2.getFlattenedVertices();
				VBO_Cube2.UpdateData(flattenedCube2Vertices.data(), flattenedCube2Vertices.size() * sizeof(GLfloat));
				
			}
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			float newPosition = Cube2.position.z + translationSpeed;

			// Check if the new position will cause a collision
			if (!Cube2.CheckCollision(Cube3))
			{
				Cube2.position.z = newPosition;

				// Update vertices and VBO
				Cube2.UpdateVertices(0.0f, 0.0f, 0.05f);
				std::vector<GLfloat> flattenedCube2Vertices = Cube2.getFlattenedVertices();
				VBO_Cube2.UpdateData(flattenedCube2Vertices.data(), flattenedCube2Vertices.size() * sizeof(GLfloat));
			}
			else
			{
				// Calculate minimum translation vector (MTV) to separate cubes
				glm::vec3 MTV = CalculateMTV(Cube2, Cube3);

				// Move the cubes away from each other using the MTV
				Cube2.position.z += MTV.z;
				Cube2.UpdateVertices(0.00f, 0.0f,0.0f);
				std::vector<GLfloat> flattenedCube2Vertices = Cube2.getFlattenedVertices();
				VBO_Cube2.UpdateData(flattenedCube2Vertices.data(), flattenedCube2Vertices.size() * sizeof(GLfloat));
			}
		}

		

		
		// Unbind VAO to prevent accidentally modifying it
		
		
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	
	
	
	
	// Delete all the objects we've created
	VAO1.Delete();
	VAO2.Delete();
	VAO3.Delete();
	VBO_Cube2.Delete();
	VBO_Cube3.Delete();
	VBO_cube.Delete();
	
	shaderProgram.Delete();
	
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}