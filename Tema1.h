#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

class Tema1 : public SimpleScene
{
	public:
		Tema1();
		~Tema1();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void RenderSky();
		void RenderCloud(float translateCircleX, float translateCircleY, std::string cloudName);
		std::string createName(std::string name, int number);
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

	protected:
		glm::mat3 modelBirdMatrix;
		glm::mat3 modelRectangleMatrixArray[8] = { };
		glm::mat3 modelGroundMatrix;
		glm::mat3 modelCircleMatrix;
		float translateRectangleDownY;
		float translateRectangleUpY;
		float translateRectangleXArray[8] = { };
		float translateBirdX, translateBirdY;
		float scaleRectangleY;
		float angularStep;
		float goUpIdx, goDownIdx;
		bool flies;
		float time;
		float maxHeight;
		float score;
		int numberOfRectangles;
		float rectangleHeight[8] = { };
		float squareBodySide = 50;
		float defaultRectangleHeight = 100;
		float rectangleWidth = 100 / 2;
		bool isDead = false;
		int spaceBetweenObstacles;

};
