#pragma once

#include "GraphicsPipeline.h"
#include "Camera.h"

class Renderer
{

public:
	
	Renderer();
	static Renderer* GetInstance();

	void InitRenderer(Window& window);

	void Render();

	void SetCamera(Camera& in_camera);
	Camera& GetCamera();

	void SetWindow(Window& in_window);
	Window& GetWindow();

	void SetGraphicsPipeline(GraphicsPipeline& in_graphicsPipeline);
	GraphicsPipeline& GetGraphicsPipeline();

	GLfloat DeltaTime();

private:

	static Renderer* instance;

	Camera camera;
	Window window;
	GLfloat lastFrame, deltaTime;
	GraphicsPipeline graphicsPipeline;
};

