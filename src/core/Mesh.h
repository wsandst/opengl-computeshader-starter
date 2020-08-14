#pragma once
#include "VBOWrapper.h"
#include "FileHandler.h"

///@brief Represents a 3D mesh. Vertices and a texture
struct Mesh
{
public:
	glm::vec3 pos;

	std::vector<GeometryVertexAttrib> vertices;
	Texture texture;

	Mesh(glm::vec3 pos, std::vector<GeometryVertexAttrib>& vertices, Texture& texture)
	{
		this->vertices = vertices;
		this->pos = pos;
		this->texture = texture;
	}

	Mesh(glm::vec3 pos, std::vector<float>& fVertices, Texture& texture)
	{
		vertices = std::vector<GeometryVertexAttrib>();
		for (size_t i = 0; i < fVertices.size(); i += 8)
		{
			vertices.push_back(GeometryVertexAttrib(fVertices[i], fVertices[i + 1], fVertices[i + 2], fVertices[i + 3], fVertices[i + 4], fVertices[i + 5], fVertices[i + 6], fVertices[i + 7]));
		}
		this->pos = pos;
		this->texture = texture;
	}
	Mesh() {};
	~Mesh() {};
};

