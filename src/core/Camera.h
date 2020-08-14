#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

///@brief A class for representing the OpenGL camera
class Camera
{
private:
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4x4 viewMatrix, projectionMatrix;

public:
	//Camera settings
	float sensitivity = 0.25f;
	float maxVelocity = 0.25f;
	float acceleration = 0.1;

	//Camera variables
	float FOV = 45.0f, yaw = -90.0f, pitch = 0.0f;
	int windowWidth, windowHeight;
	float cameraStep = 1;

	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);

	bool keyForward = false, keyBackward = false, keyRight = false, keyLeft = false, keyUp = false, keyDown = false;

	///@brief Accelerate the velocity vector along the given directions, and make sure to not exceed maxVelocity in the pos direction
	void accelerate(float x, float y, float z)
	{
		if (std::abs(velocity.x + x) <= maxVelocity)
			velocity.x = velocity.x + x;
		if (std::abs(velocity.y + y) <= maxVelocity)
			velocity.y = velocity.y + y;
		if (std::abs(velocity.z + z) <= maxVelocity)
			velocity.z = velocity.z + z;
	}

	///@brief Decelerate the velocity vector along the given directions, and make sure to not exceed maxVelocity in neg direction
	void decelerate(float x, float y, float z)
	{
		if (std::abs(velocity.x - x) <= std::abs(velocity.x))
			velocity.x = velocity.x - x;
		else
			velocity.x = 0;
		if (std::abs(velocity.y - y) <= std::abs(velocity.y))
			velocity.y = velocity.y - y;
		else
			velocity.y = 0;
		if (std::abs(velocity.z - z) <= std::abs(velocity.z))
			velocity.z = velocity.z - z;
		else
			velocity.z = 0;
	}

	///@brief Accelerate in the given direction if the given key is pressed, else decelerate in that direction
	void moveKey(bool key, float ax, float ay, float az, float vc)
	{
		if (key)
			accelerate(ax, ay, az);
		else if (vc > 0)
			decelerate(ax, ay, az);
	}

	///@brief Main move function which accelerates/decelerate the camera velocity based on pressed keys
	/// and then moves the camera based on the velocity
	void move()
	{
		moveKey(keyRight, acceleration, 0, 0, velocity.x);
		moveKey(keyLeft, -acceleration, 0, 0, -velocity.x);
		moveKey(keyUp, 0, acceleration, 0, velocity.y);
		moveKey(keyDown, 0, -acceleration, 0, -velocity.y);
		moveKey(keyForward, 0, 0, acceleration, velocity.z);
		moveKey(keyBackward, 0, 0, -acceleration, -velocity.z);

		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraStep * velocity.x;
		cameraPos += cameraUp * cameraStep * velocity.y;
		cameraPos += cameraFront * cameraStep * velocity.z;
	}

	///@brief Modify the max velocity limit of the camera
	void changeMaxVelocity(float factor)
	{
		maxVelocity *= factor;
		acceleration *= factor;
	}

	///@brief Update the front vector, which controls the camera viewing angle
	void updateView(float xOffset, float yOffset)
	{
		yaw += xOffset;
		pitch += yOffset;

		if (yaw < 0)
			yaw = 360 + yaw;

		yaw = fmod(yaw, 360.0f);

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(front);
	}

	glm::vec3& getCameraFront() {
		return cameraFront;
	}

	glm::mat4x4& getProjectionMatrix()
	{
		return projectionMatrix;
	}

	glm::mat4x4& getViewMatrix()
	{
		return viewMatrix;
	}

	///@brief Calculate the camera projection matrix based on FOV, window width and height, and front/back culling distance
	void calculateProjectionMatrix()
	{
		projectionMatrix = glm::perspective(glm::radians(FOV), (float)windowWidth / (float)windowHeight, 0.1f, 4000.0f);
	}

	///@brief Calculate the view matrix (camera rotation + position)
	void calculateViewMatrix()
	{
		viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}

	glm::vec3& getPosition()
	{
		return cameraPos;
	}

	///@brief Returns a string of the cardinal viewing direction, ie north/south/east/west
	std::string getCardinalDirection()
	{
		if (225 <= yaw && yaw <= 315) //-z
			return "NORTH";
		else if (135 <= yaw && yaw <= 225) //-x
			return "EAST";
		else if (45 <= yaw && yaw <= 135) //+z
			return "SOUTH";
		else if (yaw <= 45 || yaw >= 315) //+x
			return "WEST";
		return "ERROR";
	}

	Camera(int windowWidth, int windowHeight)
	{
		this->windowHeight = windowHeight;
		this->windowWidth = windowWidth;
		updateView(0, 0);
	}

	Camera() 
	{
		updateView(0, 0);
	};
	~Camera() {};
};

