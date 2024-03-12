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
#include "SphereCollition.h"



const unsigned int width = 800;
const unsigned int height = 800;
using namespace std;

void processInput(GLFWwindow* window);


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
	shaderProgram.Activate();

	
	
	std::vector<Player> myPokaler;
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	float maxX = 18;
	float minX = -18;
	float maxZ = 18;
	float minZ = -18;
	const int maxPokals = 8;
	int score = 0;
	float scale = -7;



	//creating our objects in the scene
	for (int i = 0; i < maxPokals; ++i) {
		float randomX = static_cast<float>(std::rand()) / RAND_MAX * (maxX - minX) + minX;
		float randomZ = static_cast<float>(std::rand()) / RAND_MAX * (maxZ - minZ) + minZ;
		Player pokal(1.0f, glm::vec3(randomX, -16.0f, randomZ), 0.75f, 1.0f, 0.0f);
		myPokaler.push_back(pokal);
	}

	Player myPlayer(1.0f, glm::vec3(0,-16,0), 0.1f, 0.0f, 0.5f, 1);

	Pokal HouseFloor(1.0f, glm::vec3(22.5f, -8.5f, 0.0f), 15.0f, 0.5f, 15.0f, 0.10f, 0.0f, 0.0f);
	HouseFloor.ConstructVBO(HouseFloor.getFlattenedVertices(), false);

	Player door(1.0f, glm::vec3(0.0f, -16.0f, -18.0f), 0.0f, 1.0f, 0.0f, 1);

	Player house(5.0f, glm::vec3(0.0f, -16.0f, -23.0f), 0.5f, 0.0f, 0.7f, 3);

	Player roomDoor(1.0f, glm::vec3(45.0f, -16.0f, -14.0f), 0.0f, 1.0f, 0.0f, 1);
;

	Player NPC(1.0f, glm::vec3(-8.0f, -16, 0.0f), 1.0f, 0.0f, 0.0f, 1);

	Player myTable1(3.5f, glm::vec3(45, -16, -6), 0.929411764f, 0.831372549f, 0.760784313f, 2);


	Pokal mainFloor(1.0f, glm::vec3(0.0f, -8.5f, 0.0f), 20.0f, 0.5f, 20.0f);
	mainFloor.ConstructVBO(mainFloor.getFlattenedVertices(),false);
	// Unbind all to prevent accidentally modifying them
	
	
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
	float scaleValue = 100.0f;
	

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	
	
	//Camera object
	Camera camera(width, height, glm::vec3(0.0f, 3.0f, 70.0f));
	
	//speed of cube
	float translationSpeed = 0.05f;
	//Collison 
	bool isColldingx = false;
	bool isColldingNegativeX = false;
	bool isColldingz = false;
	bool isColldingNegativeZ = false;
	bool isInHouse = false;


	std::vector<double> patrolPoints = { -1 , 2, 1, -2, 2, 2 }; // points for patrolling
	LSM PatrolPath(patrolPoints, patrolPoints.size() / 2); // the degree of the function, f.exa x^2

	//SphereCollition sc(myPlayer, NPC);
	glfwSwapInterval(1);
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

		NPC.Patrol(PatrolPath.getCoefficients());
		processInput(window);
		myPlayer.inputs(window);
		camera.Inputs(window);
		myPlayer.CheckCollision(NPC);
		myPlayer.CheckCollision(house);
		NPC.CheckCollision(door);
		NPC.CheckCollision(house);

		//Set render distance and FOV
		glm::mat4 viewproj= camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, myPlayer.position);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(viewproj*model));
		myPlayer.BindVAO();
		myPlayer.GetVBO().Bind();
		glDrawArrays(GL_TRIANGLES, 0, myPlayer.mVertecies.size());
		myPlayer.UnbindVAO();

		// Drawing door in room
		glm::mat4 roomDmodel = glm::mat4(1.0f);
		roomDmodel = glm::translate(roomDmodel, roomDoor.position);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(viewproj * roomDmodel));
		roomDoor.BindVAO();
		glDrawArrays(GL_TRIANGLES, 0, roomDoor.mVertecies.size());
		roomDoor.UnbindVAO();

		// Drawing main floor
		glm::mat4 mainfloormodel = glm::mat4(1.0f);
		mainfloormodel = glm::translate(mainfloormodel, mainFloor.position);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(viewproj * mainfloormodel));
		mainFloor.BindVAO();
		glDrawArrays(GL_TRIANGLES, 0, mainFloor.mVertecies.size());
		mainFloor.UnbindVAO();

		// Drawing door
		glm::mat4 doorModel = glm::mat4(1.0f);
		doorModel = glm::translate(doorModel, door.position);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(viewproj * doorModel));
		door.BindVAO();
		glDrawArrays(GL_TRIANGLES, 0, door.mVertecies.size());
		door.UnbindVAO();

		// Drawing house
		glm::mat4 houseModel = glm::mat4(1.0f);
		houseModel = glm::translate(houseModel, house.position);
		houseModel = glm::scale(houseModel, glm::vec3(5,5,5));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(viewproj* houseModel));
		house.BindVAO();
		glDrawArrays(GL_TRIANGLES, 0, house.mHouseVertecies.size());
		house.UnbindVAO();

		// Drawing floor in room/house
		glm::mat4 HouseFloorModel = glm::mat4(1.0f);
		HouseFloorModel = glm::translate(HouseFloorModel, HouseFloor.position);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(viewproj * HouseFloorModel));
		HouseFloor.BindVAO();
		glDrawArrays(GL_TRIANGLES, 0, HouseFloor.mVertecies.size());
		HouseFloor.UnbindVAO();

		// Drawing npc
		glm::mat4 NPCModel = glm::mat4(1.0f);
		NPCModel = glm::translate(NPCModel, NPC.position);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(viewproj * NPCModel));
		NPC.BindVAO();
		glDrawArrays(GL_TRIANGLES, 0, NPC.mVertecies.size());
		NPC.UnbindVAO();

		glm::mat4 TableModel = glm::mat4(1.0f);
		TableModel = glm::translate(TableModel, myTable1.position);
		//TableModel = glm::scale(TableModel, glm::vec3(0.5f,1.f,0.5f));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(viewproj * TableModel));
		myTable1.BindVAO();
		glDrawArrays(GL_TRIANGLES, 0, myTable1.mVertecies.size());
		myTable1.UnbindVAO();

		// Drawing trophies
		glm::mat4 PokalModel[maxPokals];

		for (int i = 0; i < maxPokals; ++i)
		{
			PokalModel[i] = glm::mat4(1.0f);
			PokalModel[i] = glm::translate(PokalModel[i], myPokaler[i].position);
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(viewproj * PokalModel[i]));
			myPokaler[i].BindVAO();
			glDrawArrays(GL_TRIANGLES, 0, myPokaler[i].mVertecies.size());
			myPokaler[i].UnbindVAO();

		}


		


		//glUniform1f(uniID, scaleValue);
		//
		//for (int i = 0; i < maxPokals; ++i) {
		//	myPokaler[i].BindVAO();
		//	glDrawArrays(GL_TRIANGLES, 0, myPokaler[i].mVertecies.size());
		//	myPokaler[i].UnbindVAO();
		//}






		if (myTable1.CheckCollision(myPlayer) || myPlayer.CheckCollision(myTable1))
		{
			//makes player able to move table around
		}
		if (roomDoor.CheckCollision(myTable1) || myTable1.CheckCollision(roomDoor))
		{
			//makes table able to push door
		}
		if (myTable1.CheckCollision(roomDoor) || roomDoor.CheckCollision(myTable1))
		{
			//if "if-statment" is above, makes door able to push table
			//makes door have collition with table
		}

		

		for (int i = 0; i < maxPokals; ++i) {
			if (myPokaler[i].CheckCollision(myPlayer) || myPlayer.CheckCollision(myPokaler[i])) {
				// Collision detected between player and pokal[i]
				myPokaler[i].position.y = 0;
				myPokaler[i].position.x = scale; 
				myPokaler[i].position.z = -6;
				score++;
				scale += 2; 
				std::cout << "Current Score: " << score << std::endl;
				// Do something when a collision occurs, e.g., remove the pokal or decrease player health
			}
		}
		if ((myPlayer.CheckCollision(door) || myPlayer.CheckCollision(door)) && glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			cout << "Door" << endl;
			myPlayer.position.x = 45;
			myPlayer.position.z = 8;
			camera.Position.x = myPlayer.position.x; // for le door later
			isInHouse = true;
		}
		if ((roomDoor.CheckCollision(myPlayer) || myPlayer.CheckCollision(roomDoor)) && glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {

			myPlayer.position.x = 0;
			myPlayer.position.z = 8;
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

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}