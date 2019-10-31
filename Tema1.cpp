#include "Tema1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D_T1.h"
#include "Object2D_T1.h"

using namespace std;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);
	
	// initialize tx and ty of the bird
	translateBirdX = 100;
	translateBirdY = 280;

	numberOfRectangles = 13;

	for (int i = 0; i < numberOfRectangles; i++) {
		rectangleHeight[i] = 100 + (rand() % 15  + 3) * 10 ;
		translateRectangleXArray[i] = window->GetResolution().x - i * width - i * 50;
	}
	translateRectangleDownY = 0;
	translateRectangleUpY = window->GetResolution().y;

	flies = false;
	maxHeight = translateBirdY;
	score = 0;
	
	// initialize angularStep and idx to go Up or Down
	angularStep = 0;
	goUpIdx = 0;
	goDownIdx = 0;
	
	Mesh* flappyBird = Object2D_T1::CreateBird("flappyBird", corner, squareBodySide, glm::vec3(0.6, 0.5, 0.1), true);
	AddMeshToList(flappyBird);

	// add the obstacles Up and Down
	for (int i = 0; i < numberOfRectangles; i++) {
		std::string rectangleName = "rectangleDown";
		std::string s = std::to_string(i);
		rectangleName = rectangleName + s;

		Mesh* rectangle = Object2D_T1::CreateRectangleDown(rectangleName, corner, width, rectangleHeight[i], glm::vec3(0.3, 0.5, 0.5), true);
		AddMeshToList(rectangle);

		rectangleName = "rectangleUp";
		rectangleName = rectangleName + s;

		rectangle = Object2D_T1::CreateRectangleUp(rectangleName, corner, width, rectangleHeight[i], glm::vec3(0.3, 0.5, 0.5), true);
		AddMeshToList(rectangle);
	}	

}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0.5, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
	glLineWidth(10);
	modelBirdMatrix = Transform2D_T1::Translate(translateBirdX, translateBirdY);

	// translate rectangles down
	for (int i = 0; i < numberOfRectangles; i++) {
		modelRectangleMatrixArray[i] = Transform2D_T1::Translate(translateRectangleXArray[i], translateRectangleDownY);
		if (translateRectangleXArray[i] > 0) {
			translateRectangleXArray[i] -= 2;
			modelRectangleMatrixArray[i] = Transform2D_T1::Translate(translateRectangleXArray[i], translateRectangleDownY);
		} else {
			translateRectangleXArray[i] = window->GetResolution().x - 1;
			modelRectangleMatrixArray[i] = Transform2D_T1::Translate(translateRectangleXArray[i], translateRectangleDownY);
		}
	}
	
	for (int i = 0; i < numberOfRectangles; i++) {
		std::string rectangleName = "rectangleDown";
		std::string s = std::to_string(i);
		rectangleName = rectangleName + s;
		RenderMesh2D(meshes[rectangleName], shaders["VertexColor"], modelRectangleMatrixArray[i]);
	}

	// translate rectangles up
	for (int i = 0; i < numberOfRectangles; i++) {
		modelRectangleMatrixArray[i] = Transform2D_T1::Translate(translateRectangleXArray[i], translateRectangleUpY);
		if (translateRectangleXArray[i] > 0) {
			translateRectangleXArray[i] -= 2;
			modelRectangleMatrixArray[i] = Transform2D_T1::Translate(translateRectangleXArray[i], translateRectangleUpY);
		}
		else {
			translateRectangleXArray[i] = window->GetResolution().x - 1;
			modelRectangleMatrixArray[i] = Transform2D_T1::Translate(translateRectangleXArray[i], translateRectangleUpY);
		}
	}
	for (int i = 0; i < numberOfRectangles; i++) {
		std::string rectangleName = "rectangleUp";
		std::string s = std::to_string(i);
		rectangleName = rectangleName + s;
		RenderMesh2D(meshes[rectangleName], shaders["VertexColor"], modelRectangleMatrixArray[i]);
	}

	// check for Collision Up and Down
	bool foundCollision = false;
	bool birdDeadUp = false;
	bool birdDeadDown = false;
	for (int i = 0; i < numberOfRectangles; i++) {
		bool checkUp = Object2D_T1::CheckCollision(translateBirdX, translateBirdY, squareBodySide * 2, squareBodySide,
			translateRectangleXArray[i], translateRectangleUpY - rectangleHeight[i], width, rectangleHeight[i]);

		bool checkDown = Object2D_T1::CheckCollision(translateBirdX, translateBirdY, squareBodySide * 2, squareBodySide,
			translateRectangleXArray[i], translateRectangleDownY, width, rectangleHeight[i]);

		if (checkUp) {
			foundCollision = true;
			birdDeadUp = true;
			break;
		}else if (checkDown) {
			foundCollision = true;
			birdDeadDown = true;
			break;
		}
	}

	// GAME OVER if collision
	if (foundCollision && (birdDeadDown || birdDeadUp)) {
		if (!isDead) {
			isDead = true;
			cout << "GAME OVER! Score: " << score << endl;
		}
		if (birdDeadUp) 
			goDownIdx = -400;
		else 
			goDownIdx = -30;
		
		angularStep = -0.07f;
		translateBirdY += goDownIdx;
		maxHeight = translateBirdY;
		if (translateBirdY <= 0) {
			translateBirdY = 0;
			modelBirdMatrix = Transform2D_T1::Translate(translateBirdX, translateBirdY);
		}
		else {
			modelBirdMatrix = Transform2D_T1::Translate(translateBirdX, translateBirdY)
							* Transform2D_T1::Rotate(angularStep);
		}

	}else{
		score++;
		cout << score << endl;
		if (flies == true) {
			angularStep = 0.2f;
			if (translateBirdY < maxHeight) {
				translateBirdY += 1;
				modelBirdMatrix = Transform2D_T1::Translate(translateBirdX, translateBirdY)
					* Transform2D_T1::Rotate(angularStep);
			}
			if (translateBirdY >= maxHeight) {
				flies = false;
			}
		}
		else {
			// gravity rotates the bird down
			goDownIdx = -0.3f;
			angularStep = -0.5f;
			translateBirdY += goDownIdx;
			maxHeight = translateBirdY;

			modelBirdMatrix = Transform2D_T1::Translate(translateBirdX, translateBirdY)
				* Transform2D_T1::Rotate(angularStep);
		}

	} 
	RenderMesh2D(meshes["flappyBird"], shaders["VertexColor"], modelBirdMatrix);

}

void Tema1::FrameEnd()
{

}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	
}

void Tema1::OnKeyPress(int key, int mods)
{
	if(key == GLFW_KEY_SPACE){
		flies = true;
		maxHeight += 22;
	}
}

void Tema1::OnKeyRelease(int key, int mods)
{
	// add key release event

}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)width, 0, (float)height, 0.01f, 400);
}
