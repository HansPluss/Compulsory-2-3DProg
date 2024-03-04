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
	for (Vertex& vertex : mVertecies) {
		vertex.x += Xspeed * velocity.x;
		vertex.y += Yspeed * velocity.y;
		vertex.z += Zspeed * velocity.z;	
	}
	sphere_center_x = mVertecies[0].x + sphere_radius;
	sphere_center_z = mVertecies[0].z - sphere_radius;
	
	VBO1.UpdateData(getFlattenedVertices().data(), getFlattenedVertices().size() * sizeof(GLdouble));
}

VBO Player::GetVBO()
{
	return VBO1;
}



std::vector<GLfloat> Player::getFlattenedVertices() const
{
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

bool Player::CheckCollision(const Player& otherCube)
{
	float distance_centers = std::sqrt(std::pow(sphere_center_x - otherCube.sphere_center_x, 2) +
		std::pow(sphere_center_y - otherCube.sphere_center_y, 2) +
		std::pow(sphere_center_z - otherCube.sphere_center_z, 2));

	// If the distance between centers is less than the sum of the radii, collision occurs
	if (distance_centers <= (sphere_radius + otherCube.sphere_radius)) {
		// Collision detected
		  // Collision detected
		float dx = std::abs(sphere_center_x - otherCube.sphere_center_x);
		float dy = std::abs(sphere_center_y - otherCube.sphere_center_y);
		float dz = std::abs(sphere_center_z - otherCube.sphere_center_z);

		if (dx > dz) {
			if (sphere_center_x < otherCube.sphere_center_x) {
				right = false;
			}
			else {

				left = false;

			}
		}
		else if (dy < dx && dy < dz) {
			if (sphere_center_z < otherCube.sphere_center_z) {
				down = false;
			}
			else {
				up = false;
			}
		}
	}

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




std::vector<double> computeDerivative(const std::vector<double>& coefficients) {
	std::vector<double> derivativeCoefficients;

	// Compute derivative coefficients
	for (size_t i = 1; i < coefficients.size(); ++i) {
		derivativeCoefficients.push_back(i * coefficients[i]);
	}

	return derivativeCoefficients;
}

double evaluatePolynomial(const std::vector<double>& coefficients, double x) {
	double result = 0.0;
	double power = 1.0;
	for (size_t i = 0; i < coefficients.size(); ++i) {
		result += coefficients[i] * power;
		power *= x;
	}
	return result;
}

double computeDerivativeAtPoint(const std::vector<double>& coefficients, double x) {
	std::vector<double> derivativeCoefficients = computeDerivative(coefficients);
	return evaluatePolynomial(derivativeCoefficients, x);
}


void Player::Patrol(std::vector<double> coefficients)
{
	double Derivative = computeDerivativeAtPoint(coefficients, xvalue) / 512;
	for (Vertex& vertex : mVertecies) {
		vertex.x += xspeed*20;
		vertex.y += 0;
		if (xPositiveDir) vertex.z += Derivative;
		else vertex.z -= Derivative;
	}
	xvalue += xspeed;
	if (xvalue >= 1) {
		xspeed *= -1; 
		xPositiveDir = false;
	}
	else if (xvalue <= -0.25) {
		xspeed *= -1;
		xPositiveDir = true;
	}
	VBO1.UpdateData(getFlattenedVertices().data(), getFlattenedVertices().size() * sizeof(GLdouble));
}




void Player::TableCollision(const Table& otherCube)
{
	float distance_centers = std::sqrt(std::pow(sphere_center_x - otherCube.sphere_center_x, 2) +
		std::pow(sphere_center_y - otherCube.sphere_center_y, 2) +
		std::pow(sphere_center_z - otherCube.sphere_center_z, 2));

	// If the distance between centers is less than the sum of the radii, collision occurs
	if (distance_centers <= (sphere_radius + otherCube.sphere_radius)) {
		// Collision detected
		  // Collision detected
		float dx = std::abs(sphere_center_x - otherCube.sphere_center_x);
		float dy = std::abs(sphere_center_y - otherCube.sphere_center_y);
		float dz = std::abs(sphere_center_z - otherCube.sphere_center_z);

		if (dx > dz) {
			if (sphere_center_x < otherCube.sphere_center_x) {
				right = false;
			}
			else {

				left = false;

			}
		}
		else if (dy < dx && dy < dz) {
			if (sphere_center_z < otherCube.sphere_center_z) {
				down = false;
			}
			else {
				up = false;
			}
		}
	}
	
}

bool Player::PokalCollision( Pokal& otherCube)
{
	float distance_centers = std::sqrt(std::pow(sphere_center_x - otherCube.sphere_center_x, 2) +
		std::pow(sphere_center_y - otherCube.sphere_center_y, 2) +
		std::pow(sphere_center_z - otherCube.sphere_center_z, 2));

	// If the distance between centers is less than the sum of the radii, collision occurs
	if (distance_centers <= (sphere_radius + otherCube.sphere_radius)) {
		// Collision detected
		  // Collision detected
		float dx = std::abs(sphere_center_x - otherCube.sphere_center_x);
		float dy = std::abs(sphere_center_y - otherCube.sphere_center_y);
		float dz = std::abs(sphere_center_z - otherCube.sphere_center_z);

		if (dx > dz) {
			if (sphere_center_x < otherCube.sphere_center_x) {
				right = false;
			}
			else {

				left = false;

			}
		}
		else if (dy < dx && dy < dz) {
			if (sphere_center_z < otherCube.sphere_center_z) {
				down = false;
			}
			else {
				up = false;
			}
		}
		return true;
	}
	else
	{
		up = true; 
		down = true; 
		left = true; 
		right = true; 
		return false;
	}
}

void Player::flattenVertices()
{
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
