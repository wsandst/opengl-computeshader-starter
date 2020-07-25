#pragma once
#include "Helpers.h"

#define GLEW_STATIC 
#include "GL/glew.h"

#include "SDL.h"
#include "SDL_opengl.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>

struct GeometryVertexAttrib {
	float position[3];
	float normal[3];
	float uv[2];

	GeometryVertexAttrib(float x, float y, float z, float nx, float ny, float nz, float u, float v) {
		position[0] = x;
		position[1] = y;
		position[2] = z;
		normal[0] = nx;
		normal[1] = ny;
		normal[2] = nz;
		uv[0] = u;
		uv[1] = v;
	}
	GeometryVertexAttrib(glm::vec3& pos, glm::vec3& normal, glm::vec2 uv) {
		position[0] = pos.x;
		position[1] = pos.y;
		position[2] = pos.z;
		normal[0] = normal.x;
		normal[1] = normal.y;
		normal[2] = normal.z;
		uv[0] = uv.x;
		uv[1] = uv.y;
	}
};

class GeometryVBO
{
public:
	unsigned int VBO;
	unsigned int VAO;
	glm::mat4 translation;
	int size;

	GeometryVBO() {};
	GeometryVBO(Vec3i pos, std::vector<GeometryVertexAttrib>& vertices)
	{
		create(vertices);
		setPosition(pos);
	};
	~GeometryVBO()
	{
	};
	void create(std::vector<GeometryVertexAttrib>& vertices)
	{
		size = vertices.size();
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		if (vertices.size() > 0)
		{
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GeometryVertexAttrib), &vertices[0], GL_STATIC_DRAW);
		}

		//Positions
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GeometryVertexAttrib), nullptr);
		glEnableVertexAttribArray(0);

		//Vertex normal								
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GeometryVertexAttrib), (void*)offsetof(GeometryVertexAttrib, normal));
		glEnableVertexAttribArray(1);		

		//Texture uvs
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GeometryVertexAttrib), (void*)offsetof(GeometryVertexAttrib, uv));
		glEnableVertexAttribArray(2);

	}
	void update(std::vector<GeometryVertexAttrib>& vertices)
	{
		size = vertices.size();
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		if (vertices.size() > 0)
		{
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STREAM_DRAW);
		}
	}
	void remove()
	{

	}
	void setPosition(Vec3i pos)
	{
		translation = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, pos.z));
	}
};