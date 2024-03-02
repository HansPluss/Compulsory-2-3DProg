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
#include "Pokal.h"
#include "Player.h"


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
	float r, g, b;
	std::string name;
	glm::vec3 velocity;

	
	Cube(float scale, const glm::vec3& initialPosition = glm::vec3(0.0f,0.0f,0.0f), float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f, float red = 1.0f, float green = 1.0f, float blue = 1.0f)
		: a(scale), position(initialPosition), scaleX(scaleX), scaleY(scaleY), scaleZ(scaleZ), velocity(glm::vec3(0.0f)), r(red),g(green),b(blue) {
		Vertex v0{ -a, -a, a , r, g, b };
		Vertex v1{ a, -a, a , r, g, b };
		Vertex v2{ a, a, a , r, g, b };
		Vertex v3{ -a, a, a ,  r, g, b };
		Vertex v4{ -a, -a, -a ,  r, g, b };
		Vertex v5{ a, -a, -a ,  r, g, b };
		Vertex v6{ a, a, -a ,  r, g, b };
		Vertex v7{ -a, a, -a , r, g, b };


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
	void UpdateVertices(float Xspeed,float Yspeed,float Zspeed, glm::vec3 velocity) {

		for (Vertex& vertex : mVertecies) {
			vertex.x +=  Xspeed * velocity.x;
			vertex.y +=  Yspeed * velocity.y;
			vertex.z +=  Zspeed * velocity.z;
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
						otherVertex.z * otherCube.scaleZ+ otherCube.position.z));
				if (position.x + a * scaleX > otherCube.position.x - otherCube.a * otherCube.scaleX &&
					otherCube.position.x + otherCube.a * otherCube.scaleX > position.x - a * scaleX) {
					//std::cout << "Collision X " << std::endl;
				}
				
				// Check if the distance is less than a threshold (adjust as needed)
				if (distance < 1.0f) {
					// Collision detected
					std::cout << "Collision Distance: " << distance << std::endl;
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

	Shader shaderProgram("default.vert", "default.frag");

	/*Pokal minPokal(1.0f, glm::vec3(0.0f, 4.0f, 0.0f), 1.0f, 1.0f, 1.0f, 0.0f, 0.5f, 1.0f);
	std::vector<GLfloat> pokalVert = minPokal.getFlattenedVertices();*/
	
	
	Cube mahCube(1.5f,glm::vec3(0.0f,-10.0f,0.0f),10.0f,0.5f,10.0f);
	std::vector<GLfloat> flattenedCubeVertices = mahCube.getFlattenedVertices();

	Cube Cube2(1.0f, glm::vec3(0.0f, -8.0f, 0.0f), 1.0f, 1.0f, 1.0f, 0.0f,0.5f,0.0f);
	std::vector<GLfloat> flattenedCube2Vertices = Cube2.getFlattenedVertices();

	Cube Cube3(1.0f, glm::vec3(0.0f, -8.0f, -12.0f), 14.0f, 1.0f, 1.0f,0.5f,0.0f,0.60f);
	std::vector<GLfloat> flattenedCube3Vertices = Cube3.getFlattenedVertices();

	// Generates Vertex Buffer Object and links it to cube
	//should probably have this in the class but nah, not yet
	VAO VAO1;
	VAO1.Bind();
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
	
	std::vector<Pokal> myPokaler;
	
	for (int i = 0; i < 10; ++i) {
		Pokal pokal(1.0f, glm::vec3(0.0f, -8.0f, 0.0f), 1.0f, 1.0f, 1.0f, 0.1f, 0.0f, 0.10f);
		myPokaler.push_back(pokal);


	}
	for (int i = 0; i < 10; ++i) {

		myPokaler[i].position.x += i * 2;
		myPokaler[i].position.z += i + 4;
		myPokaler[i].ConstructVBO(myPokaler[i].getFlattenedVertices(),false);
		myPokaler[i].UpdateVertices(0, 0, 0);
	}
	Player myPlayer(1.0f, glm::vec3(8.0f, -8.0f, 1.0f), 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f);
	myPlayer.ConstructVBO(myPlayer.getFlattenedVertices(),false);
	std::vector<GLfloat> playerVertices = myPlayer.getFlattenedVertices();

	Pokal HouseFloor(1.0f, glm::vec3(45.0f, -10.0f, 0.0f), 15.0f, 0.50f, 15.0f, 0.90f, 0.0f, 0.0f);
	HouseFloor.ConstructVBO(HouseFloor.getFlattenedVertices(), false);
	HouseFloor.UpdateVertices(0,0,0);
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
	Camera camera(width, height, glm::vec3(0.0f, 38.0f, 15.0f));
	// Main while loop
	float translationSpeed = 0.05f;
	bool isColldingx = false;
	bool isColldingNegativeX = false;
	bool isColldingz = false;
	bool isColldingNegativeZ = false;
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
		
		
		for (int i = 0; i < 10; ++i) {
			myPokaler[i].BindVAO();
			glDrawArrays(GL_TRIANGLES, 0, myPokaler[i].mVertecies.size());
			myPokaler[i].UnbindVAO();
		}


		myPlayer.BindVAO();
		glDrawArrays(GL_TRIANGLES, 0, myPlayer.mVertecies.size());
		myPlayer.UnbindVAO();

		HouseFloor.BindVAO();
		glDrawArrays(GL_TRIANGLES, 0, HouseFloor.mVertecies.size());
		HouseFloor.UnbindVAO();
		if (isColldingNegativeX && isColldingNegativeZ && isColldingx && isColldingz) {
			isColldingNegativeX = false;
			isColldingx = false;
			isColldingz = false;
			isColldingNegativeZ = false;

		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && myPlayer.left)
		{
			//float newPosition = Cube2.position.x - translationSpeed;
			float newpos = myPlayer.position.x - translationSpeed;
			// Check if the new position will cause a collision
			if (!isColldingNegativeX)
			{
				//Cube2.position.x = newPosition;
				myPlayer.position.x = newpos;
				



				// Update vertices and VBO
				
				glm::vec3 velocity(1, 0, 0);
				
				myPlayer.UpdateVertices(-translationSpeed, 0.0f, 0.0f, velocity);
				
				
				
				//cout << myPlayer.position.x << "  ";
				
				//Cube2.UpdateVertices(-translationSpeed, 0.0f, 0.0f, velocity);
				std::vector<GLfloat> flattenedCube2Vertices = Cube2.getFlattenedVertices();
				VBO_Cube2.UpdateData(flattenedCube2Vertices.data(), flattenedCube2Vertices.size() * sizeof(GLfloat));
				//isColldingNegativeX = false;
				//isColldingx = false;
				isColldingz = false;
				isColldingNegativeZ = false;
			}
			else {
				//isColldingNegativeX = true;
			}
			
		}
		
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && myPlayer.right)
		{
			//float newPosition = Cube2.position.x + translationSpeed;
			float newpos = myPlayer.position.x + translationSpeed;
			// Check if the new position will cause a collision
			if (!isColldingx)
			{
				//Cube2.position.x = newPosition;
				myPlayer.position.x = newpos;
				glm::vec3 velocity(1, 0, 0);
				myPlayer.UpdateVertices(translationSpeed, 0.0f, 0.0f, velocity);
				// Update vertices and VBO
				//Cube2.UpdateVertices(translationSpeed, 0.0f, 0.0f,velocity);
				std::vector<GLfloat> flattenedCube2Vertices = Cube2.getFlattenedVertices();
				VBO_Cube2.UpdateData(flattenedCube2Vertices.data(), flattenedCube2Vertices.size() * sizeof(GLfloat));
				//isColldingNegativeX = false;
				//isColldingx = false;
				isColldingz = false;
				isColldingNegativeZ = false;
			}
			else {
				
			}
			
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && myPlayer.up)
		{
			//float newPosition = Cube2.position.z - translationSpeed;
			float newpos = myPlayer.position.z - translationSpeed;
			// Check if the new position will cause a collision
			if (!isColldingNegativeZ)
			{
				//Cube2.position.z = newPosition;
				myPlayer.position.z = newpos;
				glm::vec3 velocity(0, 0, 1);
				myPlayer.UpdateVertices(0.0f, 0.0f, -translationSpeed, velocity);
				// Update vertices and VBO
				//Cube2.UpdateVertices(0.0f, 0.0f, -translationSpeed,velocity);
				std::vector<GLfloat> flattenedCube2Vertices = Cube2.getFlattenedVertices();
				VBO_Cube2.UpdateData(flattenedCube2Vertices.data(), flattenedCube2Vertices.size() * sizeof(GLfloat));
				
			}
			
			
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && myPlayer.down)
		{
			//float newPosition = Cube2.position.z + translationSpeed;
			float newpos = myPlayer.position.z + translationSpeed;
			// Check if the new position will cause a collision
			if (!isColldingz)
			{
				//Cube2.position.z = newPosition;
				myPlayer.position.z = newpos;
				glm::vec3 velocity(0, 0, 1);
				myPlayer.UpdateVertices(0.0f, 0.0f, translationSpeed, velocity);
				// Update vertices and VBO
				//Cube2.UpdateVertices(0.0f, 0.0f, translationSpeed,velocity);
				std::vector<GLfloat> flattenedCube2Vertices = Cube2.getFlattenedVertices();
				VBO_Cube2.UpdateData(flattenedCube2Vertices.data(), flattenedCube2Vertices.size() * sizeof(GLfloat));
				
			}
			
			
		}
		if (myPlayer.position.x < -3) {
			
			isColldingNegativeX = true;
			//isColldingx = false;
			//camera.Position.x = Cube2.position.x; // for le door later
		}
		else {
			isColldingNegativeX = false;
		}
		if (myPlayer.position.x > 11) {
			isColldingx = true;
			//isColldingNegativeX = false;
		}
		else {
			isColldingx = false;
		}
		if (myPlayer.position.z < -6) {

			isColldingNegativeZ = true;
			//isColldingx = false;
			//camera.Position.x = Cube2.position.x; // for le door later
		}
		else {
			isColldingNegativeZ = false;
		}
		if (myPlayer.position.z > 7) {
			isColldingz = true;
			//isColldingNegativeX = false;
		}
		else {
			isColldingz = false;
		}


		for (int i = 0; i < 10; ++i) {
			if (myPokaler[i].CheckCollision(myPlayer)) {
				// Collision detected between player and pokal[i]
				myPokaler[i].position.y += 10;
				myPokaler[i].UpdateVertices(0, 10, 0);
				std::cout << "Collision between Player and Pokal " << i << std::endl;
				// Do something when a collision occurs, e.g., remove the pokal or decrease player health
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
	myPlayer.VAO5.Delete();
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