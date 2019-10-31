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
		glm::mat3 modelRectangleMatrixArray[13] = { };
		float translateRectangleDownY;
		float translateRectangleUpY;
		float translateRectangleXArray[13] = { };
		float translateBirdX, translateBirdY;
		float scaleX, scaleY;
		float angularStep;
		float goUpIdx, goDownIdx;
		bool flies;
		float time;
		float maxHeight;
		float score;
		float numberOfRectangles;
		float rectangleHeight[13] = { };
		float squareBodySide = 50;
		float defaultHeight = 100;
		float width = 100 / 2;
};
