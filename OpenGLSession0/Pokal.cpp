#include "Pokal.h"



float Pokal::GetA()
{
	return a;
}

void Pokal::ConstructVBO(std::vector<GLfloat> flattenedVertices, bool update)
{
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

void Pokal::DeleteVBOANDVAO()
{
	VAO5.Delete();
	
}

void Pokal::UnbindVAO()
{
	VAO5.Unbind();
}

void Pokal::BindVAO()
{
	VAO5.Bind();
}

VBO Pokal::GetVBO()
{
	return VBO1;
}

void Pokal::UpdateVertices(float Xspeed, float Yspeed, float Zspeed)
{
	for (VertexPokal& vertex : mVertecies) {
		vertex.x += Xspeed;
		vertex.y += Yspeed;
		vertex.z += Zspeed;

	}

	VBO1.UpdateData(getFlattenedVertices().data(), getFlattenedVertices().size() * sizeof(GLfloat));
}

std::vector<GLfloat> Pokal::getFlattenedVertices() const
{
	std::vector<GLfloat> flattenedVertices;
	for (const VertexPokal& vertex : mVertecies) {
		flattenedVertices.push_back((vertex.x * scaleX) + position.x);
		flattenedVertices.push_back((vertex.y * scaleY) + position.y);
		flattenedVertices.push_back((vertex.z * scaleZ) + position.z);
		flattenedVertices.push_back(vertex.r);
		flattenedVertices.push_back(vertex.g);
		flattenedVertices.push_back(vertex.b);
	}
	
	return flattenedVertices;
}

bool Pokal::CheckCollision(const Player& otherCube)
{
	// Iterate through each vertex in the current cube
	for (const VertexPokal& vertex : mVertecies) {
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
			if (distance < 0.50f) {
				// Collision detected
				//std::cout << "Collision Distance: " << distance << std::endl;
				return true;
			}


		}
	}

	// No collision detected
	return false;
}


void Pokal::flattenVertices()
{
	std::vector<GLfloat> flattenedVertices;
	for (const VertexPokal& vertex : mVertecies) {
		flattenedVertices.push_back((vertex.x * scaleX) + position.x);
		flattenedVertices.push_back((vertex.y * scaleY) + position.y);
		flattenedVertices.push_back((vertex.z * scaleZ) + position.z);
		flattenedVertices.push_back(vertex.r);
		flattenedVertices.push_back(vertex.g);
		flattenedVertices.push_back(vertex.b);
	}
}
