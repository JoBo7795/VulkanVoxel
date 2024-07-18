#include "Renderer.h"

Renderer* Renderer::instance = nullptr;

Renderer* Renderer::GetInstance() {

	if (instance == nullptr) {
		instance = new Renderer();
	}

	return instance;
}

Renderer::Renderer() {


}

void Renderer::InitRenderer(Window& window) {

	graphicsPipeline.SetupGraphicsPipeline();
}

void Renderer::Render() {

	graphicsPipeline.DrawFrame(window);

}

GLfloat Renderer::DeltaTime() {

	return 0.0f;
}

void Renderer::SetCamera(Camera& in_camera) {
	camera = in_camera;
}

Camera& Renderer::GetCamera() {
	return camera;
}

void Renderer::SetGraphicsPipeline(GraphicsPipeline& in_graphicsPipeline) {

	graphicsPipeline = in_graphicsPipeline;
}

GraphicsPipeline& Renderer::GetGraphicsPipeline() {

	return graphicsPipeline;
}

void Renderer::SetWindow(Window& in_window) {

	window = in_window;
}

Window& Renderer::GetWindow() {

	return window;
}

