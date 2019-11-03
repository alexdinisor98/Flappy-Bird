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

	glm::vec3 corner = glm::vec3(0, 0, 1);
	
	// initialize tx and ty of the bird
	translateBirdX = 100;
	translateBirdY = 280;
	spaceBetweenObstacles = 110;

	numberOfRectangles = window->GetResolution().x / (rectangleWidth + spaceBetweenObstacles);

	// initialize the obstacles height randomly
	rectangleHeight[0] = 100 + (rand() % 10 + 3) * 10;
	translateRectangleXArray[0] = window->GetResolution().x;
	for (int i = 1; i < numberOfRectangles; i++) {
		rectangleHeight[i] = 100 + (rand() % 14  + 2) * 10 ;
		translateRectangleXArray[i] = window->GetResolution().x - i * rectangleWidth - i * spaceBetweenObstacles;
	}
	translateRectangleDownY = 0;
	translateRectangleUpY = window->GetResolution().y;

	flies = false;
	maxHeight = translateBirdY;

	// initialize the score
	score = 0;
	
	// initialize angularStep and idx to go Up or Down
	angularStep = 0;
	goUpIdx = 0;
	goDownIdx = 0;
	
	Mesh* flappyBird = Object2D_T1::CreateBird("flappyBird", corner, squareBodySide, glm::vec3(0.6, 0.5, 0.1), true);
	AddMeshToList(flappyBird);

	// add mesh for the obstacles Up and Down
	for (int i = 0; i < numberOfRectangles; i++) {
		std::string rectangleName = createName("rectangleDown", i);
		Mesh* rectangle = Object2D_T1::CreateRectangleDown(rectangleName, corner, rectangleWidth, rectangleHeight[i], glm::vec3(0.3, 0.5, 0.5), true);
		AddMeshToList(rectangle);

		rectangleName = createName("rectangleUp", i);
		rectangle = Object2D_T1::CreateRectangleUp(rectangleName, corner, rectangleWidth, rectangleHeight[i], glm::vec3(0.3, 0.5, 0.5), true);
		AddMeshToList(rectangle);
	}	

	// add mesh for the ground
	Mesh* rectangleGround  = Object2D_T1::CreateGroundRectangle("ground", corner, window->GetResolution().x, 35, glm::vec3(0.4, 0.25, 0), true);
	AddMeshToList(rectangleGround);

	// add mesh for the clouds
	for (int j = 0; j < 6; j++) {
		std::string cloudName = createName("cloud", j);
		float numberOfCircles = 8;
		for (int i = 0; i < numberOfCircles; i++) {
			std::string circleName = createName(cloudName, i);
			Mesh* circle = Object2D_T1::CreateCircle(circleName, corner, 100, 100, 110, 110, glm::vec3(0, 0.5, 1), true);
			AddMeshToList(circle);
		}
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

	// render the ground
	modelGroundMatrix = Transform2D_T1::Translate(0, 0);
	RenderMesh2D(meshes["ground"], shaders["VertexColor"], modelGroundMatrix);

	//render the sky
	RenderSky();

	// check for Collision Up and Down
	bool foundCollision = false;
	bool birdDeadUp = false;
	bool birdDeadDown = false;
	for (int i = 0; i < numberOfRectangles; i++) {
		bool checkUp = Object2D_T1::CheckCollision(translateBirdX, translateBirdY, squareBodySide * 2, squareBodySide,
			translateRectangleXArray[i], translateRectangleUpY - rectangleHeight[i], rectangleWidth, rectangleHeight[i]);

		bool checkDown = Object2D_T1::CheckCollision(translateBirdX, translateBirdY, squareBodySide * 2, squareBodySide,
			translateRectangleXArray[i], translateRectangleDownY, rectangleWidth, rectangleHeight[i]);
		//check for collision on the Upper Obstacles
		if (checkUp) {
			foundCollision = true;
			birdDeadUp = true;
			break;
		}else //check for collision on the Lower Obstacles
			if (checkDown) {
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
			goDownIdx = -380;
		else 
			goDownIdx = -30;
		
		// the bird goes down if collision
		angularStep = -0.06f;
		translateBirdY += goDownIdx;
		maxHeight = translateBirdY;
		if (translateBirdY <= 0) {
			// bird falls
			translateBirdY = -150;
			modelBirdMatrix = Transform2D_T1::Translate(translateBirdX, translateBirdY);
		} else {
			modelBirdMatrix = Transform2D_T1::Translate(translateBirdX, translateBirdY)
							* Transform2D_T1::Rotate(angularStep);
		}

	}else{
		if (flies == true) {
			angularStep = 0.2f;
			if (translateBirdY < maxHeight) {
				translateBirdY += 2;
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

	// translate rectangles down
	for (int i = 0; i < numberOfRectangles; i++) {
		modelRectangleMatrixArray[i] = Transform2D_T1::Translate(translateRectangleXArray[i], translateRectangleDownY);
		if (translateRectangleXArray[i] > 0) {
			translateRectangleXArray[i] -= 2;
			modelRectangleMatrixArray[i] = Transform2D_T1::Translate(translateRectangleXArray[i], translateRectangleDownY);
		}
		else {
			// if the rectangle is at X = 0 -> goes back to X = windowX - 1;
			if (!isDead) {
				score++;
				cout << score << endl;
			}
			translateRectangleXArray[i] = window->GetResolution().x - 1;
			modelRectangleMatrixArray[i] = Transform2D_T1::Translate(translateRectangleXArray[i], translateRectangleDownY);
		}
	}

	for (int i = 0; i < numberOfRectangles; i++) {
		std::string rectangleName = createName("rectangleDown", i);
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
			// if the rectangle is at X = 0 -> goes back to X = windowX;
			if (!isDead) {
				score++;
				cout << score << endl;
			}
			translateRectangleXArray[i] = window->GetResolution().x - 1;
			modelRectangleMatrixArray[i] = Transform2D_T1::Translate(translateRectangleXArray[i], translateRectangleUpY);
		}
	}
	for (int i = 0; i < numberOfRectangles; i++) {
		std::string rectangleName = createName("rectangleUp", i);
		RenderMesh2D(meshes[rectangleName], shaders["VertexColor"], modelRectangleMatrixArray[i]);
	}
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
}

// Render a cloud from 8 circles.
void Tema1::RenderCloud(float translateCircleX, float translateCircleY, std::string cloudName) {
	for (int i = 0; i < 3; i++) {
		std::string circleName = createName(cloudName, i);
		modelCircleMatrix = Transform2D_T1::Translate(translateCircleX + 25 * i, translateCircleY) * Transform2D_T1::Scale(.2f, .2f);
		RenderMesh2D(meshes[circleName], shaders["VertexColor"], modelCircleMatrix);
	}

	for (int i = 0; i < 3; i++) {
		std::string circleName = createName(cloudName, i + 3);
		modelCircleMatrix = Transform2D_T1::Translate(translateCircleX + 25 * i, translateCircleY - 20) * Transform2D_T1::Scale(.2f, .2f);
		RenderMesh2D(meshes[circleName], shaders["VertexColor"], modelCircleMatrix);
	}
	modelCircleMatrix = Transform2D_T1::Translate(translateCircleX - 10, translateCircleY - 10) * Transform2D_T1::Scale(.2f, .2f);
	std::string circleName = createName(cloudName, 6);
	RenderMesh2D(meshes[circleName], shaders["VertexColor"], modelCircleMatrix);


	modelCircleMatrix = Transform2D_T1::Translate(translateCircleX + 60, translateCircleY - 10) * Transform2D_T1::Scale(.2f, .2f);
	circleName = createName(cloudName, 7);
	RenderMesh2D(meshes[circleName], shaders["VertexColor"], modelCircleMatrix);
}

// Render the sky with 8 clouds
void Tema1::RenderSky() {

	std::string cloudName = createName("cloud", 0);
	RenderCloud(100, window->GetResolution().y - 170, cloudName);

	cloudName = createName("cloud", 1);
	RenderCloud(350, window->GetResolution().y - 100, cloudName);

	cloudName = createName("cloud", 2);
	RenderCloud(window->GetResolution().x - 450, window->GetResolution().y - 230, cloudName);

	cloudName = createName("cloud", 3);
	RenderCloud(window->GetResolution().x - 650, window->GetResolution().y - 220, cloudName);

	cloudName = createName("cloud", 4);
	RenderCloud(window->GetResolution().x - 200 , window->GetResolution().y - 180, cloudName);

	cloudName = createName("cloud", 5);
	RenderCloud(window->GetResolution().x - 400, window->GetResolution().y - 100, cloudName);
}

// create name for mesh
std::string Tema1::createName(std::string name, int number) {
	std::string s = std::to_string(number);
	return name + s;
}
