#include "Construct.h"

std::array<Vertex, 36> Construct::Cube(glm::vec3 cordinates, glm::vec3 sizeXYZ, glm::vec3 Color)
{
	std::array<Vertex, 36> CubeArray;

    Vertex v0{ -sizeXYZ.x, -sizeXYZ.y, sizeXYZ.z , Color.x, Color.y, Color.z };
    Vertex v1{ sizeXYZ.x, -sizeXYZ.y, sizeXYZ.z, Color.x, Color.y, Color.z };
    Vertex v2{ sizeXYZ.x, sizeXYZ.y, sizeXYZ.z , Color.x, Color.y, Color.z };
    Vertex v3{ -sizeXYZ.x, sizeXYZ.y, sizeXYZ.z ,  Color.x, Color.y, Color.z };
    Vertex v4{ -sizeXYZ.x, -sizeXYZ.y, -sizeXYZ.z ,  Color.x, Color.y, Color.z };
    Vertex v5{ sizeXYZ.x, -sizeXYZ.y, -sizeXYZ.z ,  Color.x, Color.y, Color.z };
    Vertex v6{ sizeXYZ.x, sizeXYZ.y, -sizeXYZ.z ,  Color.x, Color.y, Color.z };
    Vertex v7{ -sizeXYZ.x, sizeXYZ.y, -sizeXYZ.z , Color.x, Color.y, Color.z };



	// Front face
      CubeArray = {
        // Front face
        v0, v1, v3,
        v3, v1, v2,

        // Back face
        v4, v5, v7,
        v7, v5, v6,

        // Right face
        v1, v5, v2,
        v2, v5, v6,

        // Left face
        v4, v0, v7,
        v7, v0, v3,

        // Top face
        v3, v2, v7,
        v7, v2, v6,

        // Bottom face
        v4, v5, v0,
        v0, v5, v1
    };
      return CubeArray; 
}

std::array<Vertex, 36> Construct::Table(float size, glm::vec3 cordinates, glm::vec3 Color)
{
    std::array<Vertex, 36> TableArray;

    Vertex v0{ -size, -size, size , Color.x, Color.y, Color.z };
    Vertex v1{ size, -size, size, Color.x, Color.y, Color.z };
    Vertex v2{ size, size, size , Color.x, Color.y, Color.z };
    Vertex v3{ -size, size, size ,  Color.x, Color.y, Color.z };
    Vertex v4{ -size, -size, -size ,  Color.x, Color.y, Color.z };
    Vertex v5{ size, -size, -size ,  Color.x, Color.y, Color.z };
    Vertex v6{ size, size, -size ,  Color.x, Color.y, Color.z };
    Vertex v7{ -size, size, -size , Color.x, Color.y, Color.z };

    Vertex v8{ cordinates.x - (3 * size) - cordinates.x,  size, cordinates.z + (4 * size) , Color.x, Color.y, Color.z};
    Vertex v9{ cordinates.x + (3 * size) - cordinates.x,  size, cordinates.z + (4 * size) , Color.x, Color.y, Color.z };
    Vertex v10{ cordinates.x - (3 * size) - cordinates.x,  size, cordinates.z - (4 * size), Color.x, Color.y, Color.z };
    Vertex v11{ cordinates.x + (3 * size) - cordinates.x,  size, cordinates.z - (4 * size) , Color.x, Color.y, Color.z };

    TableArray = {
        // Front face
        v0, v1, v3,
        v3, v1, v2,

        // Back face
        v4, v5, v7,
        v7, v5, v6,

        // Right face
        v1, v5, v2,
        v2, v5, v6,

        // Left face
        v4, v0, v7,
        v7, v0, v3,

        // Top face
        v8, v9, v10,
        v10, v9, v11,


        // Bottom face
        v4, v5, v0,
        v0, v5, v1
    };

    return TableArray;
}

std::array<Vertex, 36> Construct::House(float size, glm::vec3 cordinates, glm::vec3 scaleXYZ, glm::vec3 Color)
{
	return std::array<Vertex, 36>();
}
