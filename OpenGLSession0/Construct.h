#pragma once
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
struct Vertex
{
	float x, y, z;
	float r, g, b;

};
class Construct
{
public:
	std::array<Vertex, 36> Cube(glm::vec3 cordinates, glm::vec3 sizeXYZ, glm::vec3 Color);
	std::array<Vertex, 36> Table(float size, glm::vec3 cordinates , glm::vec3 Color);

	//change size to fit 
	std::array<Vertex, 36> House(float size, glm::vec3 Color);
};

