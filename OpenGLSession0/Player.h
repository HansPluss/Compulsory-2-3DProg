#pragma once
#include "Resources/Shaders/VBO.h"
#include "Resources/Shaders/VAO.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include "Table.h"

using namespace std;
struct PlayerVertex
{
	float x, y, z;
	float r, g, b;

};
class Player {
private:


public:
	vector<PlayerVertex> mVertecies;
	glm::vec3 position;
	float scaleX, scaleY, scaleZ;
	float r, g, b;
	std::string name;
	glm::vec3 velocity;
	float sphere_center_x = 0;
	float sphere_center_y = 0;
	float sphere_center_z = 0;
	float sphere_radius = 0;
	bool up = true;
	bool down = true;
	bool left = true;
	bool right = true;

	
	VAO VAO5;
	VBO VBO1;
	
	
	
	Player(float scale, const glm::vec3& initialPosition = glm::vec3(0.0f, 0.0f, 0.0f), float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f, float red = 1.0f, float green = 1.0f, float blue = 1.0f)
		: a(scale), position(initialPosition), scaleX(scaleX), scaleY(scaleY), scaleZ(scaleZ), velocity(glm::vec3(0.0f)), r(red), g(green), b(blue), VBO1() {
		PlayerVertex v0{ -a, -a, a , r, g, b };
		PlayerVertex v1{ a, -a, a , r, g, b };
		PlayerVertex v2{ a, a, a , r, g, b };
		PlayerVertex v3{ -a, a, a ,  r, g, b };
		PlayerVertex v4{ -a, -a, -a ,  r, g, b };
		PlayerVertex v5{ a, -a, -a ,  r, g, b };
		PlayerVertex v6{ a, a, -a ,  r, g, b };
		PlayerVertex v7{ -a, a, -a , r, g, b };


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

	
	bool CheckCollision(const Player& otherCube);
	void TableCollision(const Table& otherCube);
	


	void flattenVertices();

public:
	float a{ 1.0f };



};