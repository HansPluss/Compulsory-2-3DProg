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

	
	
	//std::vector<Pokal> myPokaler;
	//std::srand(static_cast<unsigned>(std::time(nullptr)));
	//float maxX = 18;
	//float minX = -18;
	//float maxZ = 18;
	//float minZ = -18;
	//int maxPokals = 8;
	//int score = 0;


	//creating our objects in the scene
	//for (int i = 0; i < maxPokals; ++i) {
	//	Pokal pokal(1.0f, glm::vec3(0.0f, -8.0f, 0.0f), 1.0f, 1.0f, 1.f, 0.75f, 1.0f, 0.0f);
	//	myPokaler.push_back(pokal);


	//}
	//for (int i = 0; i < maxPokals; ++i) {

	//	float randomX = static_cast<float>(std::rand()) / RAND_MAX * (maxX - minX) + minX;
	//	float randomZ = static_cast<float>(std::rand()) / RAND_MAX * (maxZ - minZ) + minZ;

	//	myPokaler[i].position.x = randomX;
	//	myPokaler[i].position.z = randomZ;
	//	myPokaler[i].ConstructVBO(myPokaler[i].getFlattenedVertices(),false);
	//	myPokaler[i].UpdateVertices(0, 0, 0);
	//}
	Player myPlayer(1.0f, glm::vec3(0,-8,0), 1.0f, 1.0f, 1.0f, 0.10f, 0.0f, 0.50f);


	Pokal HouseFloor(1.0f, glm::vec3(45.0f, -8.5f, 0.0f), 15.0f, 0.50f, 15.0f, 0.10f, 0.0f, 0.0f);
	HouseFloor.ConstructVBO(HouseFloor.getFlattenedVertices(), false);;
	Player door(1.0f, glm::vec3(0.0f, -8.0f, -8.0f), 2.0f, 1.0f, 1.0f, 0.00f, 1.0f, 0.0f);


	Player roomDoor(1.0f, glm::vec3(45.0f, -8.0f, -14.0f), 2.0f, 1.0f, 1.0f, 0.00f, 1.0f, 0.0f);
;

	Player NPC(1.0f, glm::vec3(-8.0f, -8, 0.0f), 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.00f);


	Pokal mainFloor(1.0f, glm::vec3(0.0f, -8.5f, 0.0f), 20.0f, 0.5f, 20.0f);
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


	//std::vector<double> patrolPoints = { -1 , 2, -0.5, 0.25, 0, 0 }; // points for patrolling
	//LSM PatrolPath(patrolPoints, patrolPoints.size() / 2); // the degree of the function, f.exa x^2

	//SphereCollition sc(myPlayer, NPC);

	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		
		myPlayer.inputs(window);
		camera.Inputs(window);
		myPlayer.CheckCollision(NPC);
		//Set render distance and FOV
		glm::mat4 viewproj= camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, myPlayer.position);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(viewproj*model));
		myPlayer.BindVAO();
		myPlayer.GetVBO().Bind();
		glDrawArrays(GL_TRIANGLES, 0, myPlayer.mVertecies.size());
		myPlayer.UnbindVAO();

		glm::mat4 roomDmodel = glm::mat4(1.0f);
		roomDmodel = glm::translate(roomDmodel, roomDoor.position);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(viewproj * roomDmodel));
		roomDoor.BindVAO();
		glDrawArrays(GL_TRIANGLES, 0, roomDoor.mVertecies.size());
		roomDoor.UnbindVAO();

		glm::mat4 mainfloormodel = glm::mat4(1.0f);
		mainfloormodel = glm::translate(mainfloormodel, mainFloor.position);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(viewproj * mainfloormodel));
		mainFloor.BindVAO();
		glDrawArrays(GL_TRIANGLES, 0, mainFloor.mVertecies.size());
		mainFloor.UnbindVAO();

		glm::mat4 doorModel = glm::mat4(1.0f);
		doorModel = glm::translate(doorModel, door.position);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(viewproj * doorModel));
		door.BindVAO();
		glDrawArrays(GL_TRIANGLES, 0, door.mVertecies.size());
		door.UnbindVAO();

		glm::mat4 HouseFloorModel = glm::mat4(1.0f);
		HouseFloorModel = glm::translate(HouseFloorModel, HouseFloor.position);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(viewproj * HouseFloorModel));
		HouseFloor.BindVAO();
		glDrawArrays(GL_TRIANGLES, 0, HouseFloor.mVertecies.size());
		HouseFloor.UnbindVAO();

		glm::mat4 NPCModel = glm::mat4(1.0f);
		NPCModel = glm::translate(NPCModel, NPC.position);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(viewproj * NPCModel));
		NPC.BindVAO();
		glDrawArrays(GL_TRIANGLES, 0, NPC.mVertecies.size());
		NPC.UnbindVAO();


		


		//NPC
		//NPC.Patrol(PatrolPath.getCoefficients());
		///*sc;*/


		//glUniform1f(uniID, scaleValue);
		//
		//for (int i = 0; i < maxPokals; ++i) {
		//	myPokaler[i].BindVAO();
		//	glDrawArrays(GL_TRIANGLES, 0, myPokaler[i].mVertecies.size());
		//	myPokaler[i].UnbindVAO();
		//}







		

		//for (int i = 0; i < maxPokals; ++i) {
		//	if (myPokaler[i].CheckCollision(myPlayer) /*|| myPlayer.PokalCollision(myPokaler[i])*/) {
		//		// Collision detected between player and pokal[i]
		//		myPokaler[i].position.y += 10;
		//		myPokaler[i].position.z = -6;
		//		myPokaler[i].sphere_center_y += 10;
		//		myPokaler[i].sphere_center_z = -6;
		//		myPokaler[i].UpdateVertices(0, 10, 0);
		//		score++;
		//		std::cout << "Current Score: " << score << std::endl;
		//		// Do something when a collision occurs, e.g., remove the pokal or decrease player health
		//	}
		//}
		//if (myPlayer.CheckCollision(door)) {
		//	cout << "Door" << endl;
		//	myPlayer.position.x = 45;
		//	myPlayer.UpdateVertices(0, 0, 0, glm::vec3(0, 0, 0));
		//	camera.Position.x = myPlayer.position.x; // for le door later
		//	myPlayer.sphere_center_x = myPlayer.mVertecies[1].x + myPlayer.a;
		//	myPlayer.sphere_center_z = myPlayer.mVertecies[1].z - myPlayer.a;
		//	isInHouse = true;
		//}
		//if (roomDoor.CheckCollision(myPlayer)) {

		//	myPlayer.position.x = 0;
		//	myPlayer.position.z = 8;
		//	myPlayer.UpdateVertices(0, 0, 0, glm::vec3(0, 0, 0));
		//	camera.Position.x = myPlayer.position.x;
		//	myPlayer.sphere_center_x = myPlayer.mVertecies[1].x + myPlayer.a;
		//	myPlayer.sphere_center_z = myPlayer.mVertecies[1].z - myPlayer.a;
		//	isInHouse = false;
		//}
		
		
		
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