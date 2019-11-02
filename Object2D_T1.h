#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>


namespace Object2D_T1
{

	// Create bird and obstacles with given bottom left corner, length and color
	Mesh* CreateBird(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateRectangleDown(std::string name, glm::vec3 leftBottomCorner, float width, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateRectangleUp(std::string name, glm::vec3 leftBottomCorner, float width, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateGroundRectangle(std::string name, glm::vec3 leftBottomCorner, float width, float length, glm::vec3 color, bool fill = false);
	bool CheckCollision(float birdPositionX, float birdPositionY, float birdWidth, float birdHeight, 
		float obstaclePositionX, float obstaclePositionY, float obstacleWidth, float obstacleHeight);
	Mesh* CreateCircle(std::string name, glm::vec3 leftBottomCorner, float x, float y, float xcenter, float ycenter, glm::vec3 color, bool fill = false);
	
}

