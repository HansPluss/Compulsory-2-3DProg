#pragma once
#include "Resources/Shaders/VBO.h"
#include "Resources/Shaders/VAO.h"
#include <vector>
#include <array>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include "Table.h"
#include "Pokal.h"
#include "Construct.h"

class Pokal;


using namespace std;
//struct PlayerVertex
//{
//	double x, y, z;
//	float r, g, b;
//
//};
class Player {
private:


public:
std::array<Vertex, 36> mVertecies;
	glm::vec3 position;
	float scaleX, scaleY, scaleZ;
	float r, g, b;
	glm::vec3 velocity;
	float sphere_center_x = 0;
	float sphere_center_y = 0;
	float sphere_center_z = 0;
	float sphere_radius = 0;
	bool up = true;
	bool down = true;
	bool left = true;
	bool right = true;
	bool move = true; 

	
	VAO VAO5;
	VBO VBO1;
	
	
	
	Player(float scale, const glm::vec3& initialPosition, float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f, float red = 1.0f, float green = 1.0f, float blue = 1.0f)
		: a(scale), position(initialPosition), scaleX(scaleX), scaleY(scaleY), scaleZ(scaleZ), velocity(glm::vec3(0.0f)), r(red), g(green), b(blue), VBO1() 
	{
		Construct con; 

		//Cube
		mVertecies = con.Cube(initialPosition, glm::vec3(scaleX, scaleY, scaleZ), glm::vec3(red, green, blue)); 
		//Table
		//mVertecies = con.Table(a, initialPosition, glm::vec3(red, green, blue));

		//collitionSphere
		sphere_center_x = initialPosition.x;
		sphere_center_y = initialPosition.y;
		sphere_center_z = initialPosition.z;
		sphere_radius = a; 


		flattenVertices();
	}
	float GetA();
	std::vector<GLfloat> getFlattenedVertices() const;
	void ConstructVBO(std::vector<GLfloat> flattenedVertices, bool update);
	void DeleteVBOANDVAO();
	void UnbindVAO();
	void BindVAO();
	void UpdateVertices(float Xspeed, float Yspeed, float Zspeed, glm::vec3 velocity);
	VBO GetVBO();

	void Patrol(std::vector<double> coefficients);
	
	bool CheckCollision( Player& otherCube);
	void TableCollision(const Table& otherCube);
	bool PokalCollision(Pokal& otherCube);
	


	void flattenVertices();

public:
	float a{ 1.0f };
	double xvalue = 0;
	double xspeed = 0.001;
	bool xPositiveDir = true;


};