#include "WindowCallback.h"
#include "SceneDescription.h"
#include "Renderer.h"

bool stopCamMovement = false;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	

	if (stopCamMovement)
		return;

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

	if (stopCamMovement)
		return;
	
		glm::vec3 camPos = Renderer::GetInstance()->GetCamera().GetPosition();
		glm::vec3 clickDir = Renderer::GetInstance()->GetCamera().GetDirection();

		Physics::Ray ray = Physics::Ray(camPos,clickDir,1.0f);

		GameObject posMarker;

		posMarker.position = ray.origin + ray.direction;
		posMarker.modelId = MARKER_SPHERE;
		//GameObjectManager::GetInstance()->AppendGameObjectToQueue(posMarker);

		double minDist = -1;
		glm::vec3 clickIndex;
		bool first = true, hit = false;
		double currDist;
		Physics::BoundingBox bb;
		glm::vec3 collPoint;
		uint8_t cubeSide;
		uint8_t closestCubeSide = -1;
		glm::vec3 finalCollPoint;

		bool leftClick = button == GLFW_MOUSE_BUTTON_LEFT;

		if (action == GLFW_RELEASE) {

			for (int i = 0; i < VOXEL_GRID_LENGTH; i++) {
				for (int j = 0; j < VOXEL_GRID_HEIGHT; j++) {
					for (int k = 0; k < VOXEL_GRID_DEPTH; k++) {

						int x = i, y = (VOXEL_GRID_HEIGHT - 1) -j, z = k;

						bb.xMin = glm::vec3(x, y, z).x;
						bb.xMax = glm::vec3(x + VOXEL_BOX_DIM_SIZE, y, z).x;
						bb.yMin = glm::vec3(x, y, z).y;
						bb.yMax = glm::vec3(x, y + VOXEL_BOX_DIM_SIZE, z).y;
						bb.zMin = glm::vec3(x, y, z).z;
						bb.zMax = glm::vec3(x, y, z + VOXEL_BOX_DIM_SIZE).z;

						glm::vec3 origVec = ray.origin - glm::vec3(x, y, z);

						float dotProduct = glm::dot(glm::normalize(ray.direction), origVec);

						// skip block if it's behind the camera
						if (!(dotProduct <= 0))
							continue;

						if (Physics::CheckRayBoxCollision(ray, bb, collPoint, cubeSide)) {

							currDist = glm::distance(ray.origin, collPoint);

							if ((first || (minDist > currDist)) && ((Scene::GetVoxelAtIndex(glm::vec3(x, y, z)) != 0))) {
								finalCollPoint = collPoint;
								minDist = currDist;
								clickIndex = glm::vec3(x, y, z);
								first = false;
								hit = true;
								closestCubeSide = cubeSide;
							}

						}
					}
				}
			}

			//posMarker.position = ray.origin + normalize(ray.direction) * minDist;



			if (hit) {
				posMarker.position = finalCollPoint;
				//posMarker.position = ray.origin + normalize(ray.direction) * minDist;
				GameObjectManager::GetInstance()->AppendGameObjectToQueue(posMarker);
				if (leftClick) {
					Scene::ChangeVoxelAtIndex(clickIndex, 0);
				}
				if (!leftClick) {

					Scene::voxelMesh.AddCubeToCubeSide(clickIndex, closestCubeSide);
				}


			}

		}

}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	auto camRef = Renderer::GetInstance()->GetCamera();
	auto playerPos = camRef.GetPosition();

	if (key == GLFW_KEY_X && action == GLFW_RELEASE) {

		stopCamMovement = !stopCamMovement;
	}


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