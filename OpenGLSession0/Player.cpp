#include "Player.h"
#include <iostream>




float Player::GetA()
{
    return a;
}

void Player::ConstructVBO(std::vector<GLfloat> flattenedVertices, bool update)
{
	
	

	// Update VBO data if necessary
	if (update) {
		VBO1.UpdateData(flattenedVertices.data(), flattenedVertices.size() * sizeof(GLfloat));
	}
	else {
		VAO5.Bind();
		VBO1.UpdateData(flattenedVertices.data(), flattenedVertices.size() * sizeof(GLfloat));
		VAO5.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
		VAO5.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		VAO5.Unbind();
		VBO1.Unbind();
	}
	
	
}

void Player::DeleteVBOANDVAO()
{
	VAO5.Delete();

}

void Player::UnbindVAO()
{
	VAO5.Unbind();
}

void Player::BindVAO()
{
	VAO5.Bind();
}

void Player::UpdateVertices(float Xspeed, float Yspeed, float Zspeed, glm::vec3 velocity)
{
	for (PlayerVertex& vertex : mVertecies) {
		vertex.x += Xspeed * velocity.x;
		vertex.y += Yspeed * velocity.y;
		vertex.z += Zspeed * velocity.z;
		
	}
	
	VBO1.UpdateData(getFlattenedVertices().data(), getFlattenedVertices().size() * sizeof(GLfloat));
}

VBO Player::GetVBO()
{
	return VBO1;
}

std::vector<GLfloat> Player::getFlattenedVertices() const
{
	std::vector<GLfloat> flattenedVertices;
	for (const PlayerVertex& vertex : mVertecies) {
		flattenedVertices.push_back((vertex.x * scaleX) + position.x);
		flattenedVertices.push_back((vertex.y * scaleY) + position.y);
		flattenedVertices.push_back((vertex.z * scaleZ) + position.z);
		flattenedVertices.push_back(vertex.r);
		flattenedVertices.push_back(vertex.g);
		flattenedVertices.push_back(vertex.b);
	}

	return flattenedVertices;
}

bool Player::CheckCollision(const Player& otherCube) const
{
	for (const PlayerVertex& vertex : mVertecies) {
		// Iterate through each vertex in the other cube
		for (const PlayerVertex& otherVertex : otherCube.mVertecies) {
			// Calculate the distance between the two vertices
			float distance = glm::length(glm::vec3(vertex.x * scaleX + position.x,
				vertex.y * scaleY + position.y,
				vertex.z * scaleZ + position.z) -
				glm::vec3(otherVertex.x * otherCube.scaleX + otherCube.position.x,
					otherVertex.y * otherCube.scaleY + otherCube.position.y,
					otherVertex.z * otherCube.scaleZ + otherCube.position.z));
			if (position.x + a * scaleX > otherCube.position.x - otherCube.a * otherCube.scaleX &&
				otherCube.position.x + otherCube.a * otherCube.scaleX > position.x - a * scaleX) {
				//std::cout << "Collision X " << std::endl;
			}

			// Check if the distance is less than a threshold (adjust as needed)
			if (distance < 1.0f) {
				// Collision detected
				//std::cout << "Collision Distance: " << distance << std::endl;
				return true;
			}


		}
	}

	// No collision detected
	return false;
}

void Player::flattenVertices()
{
	std::vector<GLfloat> flattenedVertices;
	for (const PlayerVertex& vertex : mVertecies) {
		flattenedVertices.push_back((vertex.x * scaleX) + position.x);
		flattenedVertices.push_back((vertex.y * scaleY) + position.y);
		flattenedVertices.push_back((vertex.z * scaleZ) + position.z);
		flattenedVertices.push_back(vertex.r);
		flattenedVertices.push_back(vertex.g);
		flattenedVertices.push_back(vertex.b);
	}
}
