#include "Object2D_T1.h"

#include <Core/Engine.h>

Mesh* Object2D_T1::CreateBird(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill) {
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		// draw the body of the bird
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(0, length, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),

		// draw the head of the bird
		VertexFormat(corner + glm::vec3(length, 0.2 * length, 0), color),
		VertexFormat(corner + glm::vec3(length, 0.8 * length, 0), color),
		VertexFormat(corner + glm::vec3(1.65 * length, 0.8 * length, 0), color),
		VertexFormat(corner + glm::vec3(1.65 * length, 0.2 * length, 0), color),

		//draw the beak of the bird
		VertexFormat(corner + glm::vec3(1.65 * length, 0.35 * length, 0), glm::vec3(0.9, 0.2, 0.2)),
		VertexFormat(corner + glm::vec3(1.65 * length, 0.68 * length, 0), glm::vec3(0.9, 0.2, 0.2)),
		VertexFormat(corner + glm::vec3(2 * length, 0.52 * length, 0), glm::vec3(0.9, 0.2, 0.2)),

		//draw wing of the bird
		VertexFormat(corner + glm::vec3(0.3 * length, 0.6 * length, 1), glm::vec3(0.9, 0.2, 0.2)),
		VertexFormat(corner + glm::vec3(0.6 * length, 0.6 * length, 1), glm::vec3(0.9, 0.2, 0.2)),
		VertexFormat(corner + glm::vec3(0.45 * length, 0.4 * length, 1), glm::vec3(0.9, 0.2, 0.2))
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2,
		2, 3, 0,
		4, 5, 6,
		6, 7, 4,
		8, 9, 10,
		11, 12, 13};
	
	if (!fill) {
		square->SetDrawMode(GL_FILL);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Object2D_T1::CreateRectangleDown(std::string name, glm::vec3 leftBottomCorner, float width, float length, glm::vec3 color, bool fill) {
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(0, length, 0), glm::vec3(0.3, 1, 0.1)),
		VertexFormat(corner + glm::vec3(width, length, 0), glm::vec3(0.2, 0.8, 0.2)),
		VertexFormat(corner + glm::vec3(width, 0, 0), glm::vec3(0.2, 0.5, 0.2)),

		/*VertexFormat(corner + glm::vec3(0, length - 10, 1), glm::vec3(0, 0, 1)),
		VertexFormat(corner + glm::vec3(width, length - 10, 1), glm::vec3(0, 0, 1)),
		VertexFormat(corner + glm::vec3(0, length -10 , 1), glm::vec3(0, 0, 1)),*/
	};

	Mesh* rectangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2,
		2, 3, 0};

	if (!fill) {
		rectangle->SetDrawMode(GL_FILL);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	rectangle->InitFromData(vertices, indices);
	return rectangle;

}

Mesh* Object2D_T1::CreateRectangleUp(std::string name, glm::vec3 leftBottomCorner, float width, float length, glm::vec3 color, bool fill) {
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{

		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(0, -length, 0), glm::vec3(0.3, 1, 0.1)),
		VertexFormat(corner + glm::vec3(width, -length, 0), glm::vec3(0.2, 0.8, 0.2)),
		VertexFormat(corner + glm::vec3(width, 0, 0), glm::vec3(0.2, 0.5, 0.2)),


	};

	Mesh* rectangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2,
		2, 3, 0 };

	if (!fill) {
		rectangle->SetDrawMode(GL_FILL);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	rectangle->InitFromData(vertices, indices);
	return rectangle;

}

// AABB - AABB collision
bool Object2D_T1::CheckCollision(float birdPositionX, float birdPositionY, float birdWidth, float birdHeight,
	float obstaclePositionX, float obstaclePositionY, float obstacleWidth, float obstacleHeight) {

	// Collision x-axis?
	bool collisionX = birdPositionX + birdWidth >= obstaclePositionX &&
		obstaclePositionX + obstacleWidth >= birdPositionX;

	// Collision y-axis?
	bool collisionY = birdPositionY + birdHeight >= obstaclePositionY &&
		obstaclePositionY + obstacleHeight >= birdPositionY;

	// Collision only if on both axes
	return collisionX && collisionY;
}

