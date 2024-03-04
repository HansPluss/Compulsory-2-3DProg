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
#include <cstdlib> 
#include <ctime> 


#include "Resources/Shaders/shaderClass.h"
#include "Resources/Shaders/VAO.h"
#include "Resources/Shaders/VBO.h"
#include "Resources/Shaders/EBO.h"
#include "Camera.h"
#include "Pokal.h"
#include "Player.h"
#include "LSM.h"



const unsigned int width = 800;
const unsigned int height = 800;
using namespace std;




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

	
	glViewport(0, 0, width, height);

	Shader shaderProgram("default.vert", "default.frag");

	
	
	std::vector<Pokal> myPokaler;
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	float maxX = 18;
	float minX = -18;
	float maxZ = 18;
	float minZ = -18;
	int maxPokals = 8;
	int score = 0;


	//creating our objects in the scene
	for (int i = 0; i < maxPokals; ++i) {
		Pokal pokal(1.0f, glm::vec3(0.0f, -8.0f, 0.0f), 1.0f, 1.0f, 1.f, 0.75f, 1.0f, 0.0f);
		myPokaler.push_back(pokal);


	}
	for (int i = 0; i < maxPokals; ++i) {

		float randomX = static_cast<float>(std::rand()) / RAND_MAX * (maxX - minX) + minX;
		float randomZ = static_cast<float>(std::rand()) / RAND_MAX * (maxZ - minZ) + minZ;

		myPokaler[i].position.x = randomX;
		myPokaler[i].position.z = randomZ;
		myPokaler[i].ConstructVBO(myPokaler[i].getFlattenedVertices(),false);
		myPokaler[i].UpdateVertices(0, 0, 0);
	}
	Player myPlayer(1.0f, glm::vec3(8.0f, -8.0f, 1.0f), 1.0f, 1.0f, 1.0f, 0.10f, 0.0f, 0.50f);
	myPlayer.ConstructVBO(myPlayer.getFlattenedVertices(),false);
	std::vector<GLfloat> playerVertices = myPlayer.getFlattenedVertices();

	Pokal HouseFloor(1.0f, glm::vec3(45.0f, -10.0f, 0.0f), 15.0f, 0.50f, 15.0f, 0.10f, 0.0f, 0.0f);
	HouseFloor.ConstructVBO(HouseFloor.getFlattenedVertices(), false);
	HouseFloor.UpdateVertices(0,0,0);
	Player door(1.0f, glm::vec3(0.0f, -8.0f, -12.0f), 2.0f, 1.0f, 1.0f, 0.00f, 1.0f, 0.0f);
	door.ConstructVBO(door.getFlattenedVertices(), false);

	Player roomDoor(1.0f, glm::vec3(45.0f, -8.0f, -14.0f), 2.0f, 1.0f, 1.0f, 0.00f, 1.0f, 0.0f);
	roomDoor.ConstructVBO(roomDoor.getFlattenedVertices(), false);

	Player NPC(1.0f, glm::vec3(-8.0f, -8.0f, 0.0f), 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.00f);
	NPC.ConstructVBO(NPC.getFlattenedVertices(), false);
	std::vector<GLfloat> NPCVertices = NPC.getFlattenedVertices();

	Pokal mainFloor(1.0f, glm::vec3(0.0f, -10.0f, 0.0f), 20.0f, 0.5f, 20.0f);
	mainFloor.ConstructVBO(mainFloor.getFlattenedVertices(),false);
	// Unbind all to prevent accidentally modifying them
	
	
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
	float scaleValue = 100.0f;
	
	// Variables that help the rotation of the pyramid
	

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	
	
	//Camera object
	Camera camera(width, height, glm::vec3(0.0f, 3.0f, 55.0f));
	
	//speed of cube
	float translationSpeed = 0.05f;
	//Collison 
	bool isColldingx = false;
	bool isColldingNegativeX = false;
	bool isColldingz = false;
	bool isColldingNegativeZ = false;
	bool isInHouse = false;


	std::vector<double> patrolPoints = { -1 , 2, -0.5, 0.25, 0, 0 }; // points for patrolling
	LSM PatrolPath(patrolPoints, patrolPoints.size() / 2); // the degree of the function, f.exa x^2

	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		
		camera.Inputs(window);
		//Set render distance and FOV
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");


		//NPC
		NPC.Patrol(PatrolPath.getCoefficients());


		glUniform1f(uniID, scaleValue);
		
		for (int i = 0; i < maxPokals; ++i) {
			myPokaler[i].BindVAO();
			glDrawArrays(GL_TRIANGLES, 0, myPokaler[i].mVertecies.size());
			myPokaler[i].UnbindVAO();
		}


		myPlayer.BindVAO();
		glDrawArrays(GL_TRIANGLES, 0, myPlayer.mVertecies.size());
		myPlayer.UnbindVAO();

		NPC.BindVAO();
		glDrawArrays(GL_TRIANGLES, 0, NPC.mVertecies.size());
		NPC.UnbindVAO();

		HouseFloor.BindVAO();
		glDrawArrays(GL_TRIANGLES, 0, HouseFloor.mVertecies.size());
		HouseFloor.UnbindVAO();


		door.BindVAO();
		glDrawArrays(GL_TRIANGLES, 0, door.mVertecies.size());
		door.UnbindVAO();

		roomDoor.BindVAO();
		glDrawArrays(GL_TRIANGLES, 0, roomDoor.mVertecies.size());
		roomDoor.UnbindVAO();

		mainFloor.BindVAO();
		glDrawArrays(GL_TRIANGLES, 0, mainFloor.mVertecies.size());
		mainFloor.UnbindVAO();

		if (isColldingNegativeX && isColldingNegativeZ && isColldingx && isColldingz) {
			isColldingNegativeX = false;
			isColldingx = false;
			isColldingz = false;
			isColldingNegativeZ = false;

		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && myPlayer.left)
		{
			
			float newpos = myPlayer.position.x - translationSpeed;
			// Check if the new position will cause a collision
			if (!isColldingNegativeX)
			{
				//Cube2.position.x = newPosition;
				myPlayer.position.x = newpos;
		
				// Update vertices and VBO
				glm::vec3 velocity(1, 0, 0);
				myPlayer.UpdateVertices(-translationSpeed, 0.0f, 0.0f, velocity);

				
			}
			
			
		}
		
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && myPlayer.right)
		{
			
			float newpos = myPlayer.position.x + translationSpeed;
			// Check if the new position will cause a collision
			if (!isColldingx)
			{
				//Cube2.position.x = newPosition;
				myPlayer.position.x = newpos;
				glm::vec3 velocity(1, 0, 0);
				myPlayer.UpdateVertices(translationSpeed, 0.0f, 0.0f, velocity);
				
			}
			
			
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && myPlayer.up)
		{
			
			float newpos = myPlayer.position.z - translationSpeed;
			// Check if the new position will cause a collision
			if (!isColldingNegativeZ)
			{
				//Cube2.position.z = newPosition;
				myPlayer.position.z = newpos;
				glm::vec3 velocity(0, 0, 1);
				myPlayer.UpdateVertices(0.0f, 0.0f, -translationSpeed, velocity);
				// Update vertices and VBO
				
				
				
			}
			
			
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && myPlayer.down)
		{
			
			float newpos = myPlayer.position.z + translationSpeed;
			// Check if the new position will cause a collision
			if (!isColldingz)
			{
				//Cube2.position.z = newPosition;
				myPlayer.position.z = newpos;
				glm::vec3 velocity(0, 0, 1);
				myPlayer.UpdateVertices(0.0f, 0.0f, translationSpeed, velocity);
			
				
				
			}
			
			
		}
		if (!isInHouse) {
			//if (myPlayer.position.x < -3) {

			//	isColldingNegativeX = true;
			//	//isColldingx = false;

			//}
			//else {
			//	isColldingNegativeX = false;
			//}
			//if (myPlayer.position.x > 11) {
			//	isColldingx = true;
			//	//isColldingNegativeX = false;
			//}
			//else {
			//	isColldingx = false;
			//}
			//if (myPlayer.position.z < -6) {

			//	isColldingNegativeZ = true;
			//	//isColldingx = false;
			//	//camera.Position.x = Cube2.position.x; // for le door later
			//}
			//else {
			//	isColldingNegativeZ = false;
			//}
			//if (myPlayer.position.z > 7) {
			//	isColldingz = true;
			//	//isColldingNegativeX = false;
			//}
			//else {
			//	isColldingz = false;
			//}

		}
		else {
		
		}
		

		for (int i = 0; i < maxPokals; ++i) {
			if (myPokaler[i].CheckCollision(myPlayer) /*|| myPlayer.PokalCollision(myPokaler[i])*/) {
				// Collision detected between player and pokal[i]
				myPokaler[i].position.y += 10;
				myPokaler[i].position.z = -6;
				myPokaler[i].sphere_center_y += 10;
				myPokaler[i].sphere_center_z = -6;
				myPokaler[i].UpdateVertices(0, 10, 0);
				score++;
				std::cout << "Current Score: " << score << std::endl;
				// Do something when a collision occurs, e.g., remove the pokal or decrease player health
			}
		}
		if (door.CheckCollision(myPlayer)) {
			cout << "Door" << endl;
			myPlayer.position.x = 45;
			myPlayer.UpdateVertices(0, 0, 0, glm::vec3(0, 0, 0));
			camera.Position.x = myPlayer.position.x; // for le door later
			isInHouse = true;
		}
		if (roomDoor.CheckCollision(myPlayer)) {

			myPlayer.position.x = 0;
			myPlayer.position.z = 8;
			myPlayer.UpdateVertices(0, 0, 0, glm::vec3(0, 0, 0));
			camera.Position.x = myPlayer.position.x;
			isInHouse = false;
		}
		
		
		
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	
	
	
	
	// Delete all the objects we've created
	
	myPlayer.VAO5.Delete();
	NPC.VAO5.Delete();
	
	shaderProgram.Delete();
	
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}