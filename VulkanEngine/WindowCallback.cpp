#include "WindowCallback.h"
#include "Renderer.h"
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	
	Renderer* renderRef = Renderer::GetInstance();
	
	glm::vec3 direction = renderRef->GetCamera().GetDirection();

	
	direction.x = cos(glm::radians(xpos)) * cos(glm::radians(-ypos));
	direction.y = sin(glm::radians(ypos));
	direction.z = sin(glm::radians(xpos)) * cos(glm::radians(ypos));
	
	renderRef->GetCamera().SetDirection(glm::normalize(direction));
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	
	auto camRef = Renderer::GetInstance()->GetCamera();
	Renderer::GetInstance()->GetCamera().SetCameraSpeed(camRef.GetCameraSpeed()+yoffset*.1);
	

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	auto camRef = Renderer::GetInstance()->GetCamera();
	auto playerPos = camRef.GetPosition();

	const float cameraSpeed = camRef.GetCameraSpeed();
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key == GLFW_KEY_W && action != GLFW_RELEASE) {

		playerPos += camRef.GetDirection()* cameraSpeed;
		Renderer::GetInstance()->GetCamera().SetPosition(playerPos);

	}
	if (key == GLFW_KEY_S && action != GLFW_RELEASE) {

		playerPos -= camRef.GetDirection() * cameraSpeed;
		Renderer::GetInstance()->GetCamera().SetPosition(playerPos);
	}

	if (key == GLFW_KEY_A && action != GLFW_RELEASE) {

		playerPos -= glm::cross(camRef.GetDirection(),camRef.GetUp()) * cameraSpeed;
		Renderer::GetInstance()->GetCamera().SetPosition(playerPos);

	}
	if (key == GLFW_KEY_D && action != GLFW_RELEASE) {

		playerPos += glm::cross(camRef.GetDirection(), camRef.GetUp()) * cameraSpeed;
		Renderer::GetInstance()->GetCamera().SetPosition(playerPos);
	}
	if (key == GLFW_KEY_SPACE && action != GLFW_RELEASE) {

		playerPos += camRef.GetUp() * cameraSpeed;
		Renderer::GetInstance()->GetCamera().SetPosition(playerPos);
	}
	if (key == GLFW_KEY_LEFT_ALT && action != GLFW_RELEASE) {

		playerPos -= camRef.GetUp() * cameraSpeed;
		Renderer::GetInstance()->GetCamera().SetPosition(playerPos);
	}
}