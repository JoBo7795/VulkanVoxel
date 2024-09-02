#include "WindowCallback.h"
#include "SceneDescription.h"
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
	


		glm::vec3 camPos = Renderer::GetInstance()->GetCamera().GetPosition();
		glm::vec3 clickDir = Renderer::GetInstance()->GetCamera().GetDirection();

		Physics::Ray ray = Physics::Ray(camPos,clickDir,1.0f);

		GameObject posMarker;

		posMarker.position = ray.origin + ray.direction;
		posMarker.modelId = MARKER_SPHERE;
		GameObjectManager::GetInstance()->AppendGameObjectToQueue(posMarker);

		int c = 0;

		float minDist;
		glm::vec3 clickIndex;
		bool first = true, hit = false;
		float currDist;
		Physics::BoundingBox bb;
		glm::vec3 collPoint;
		uint8_t cubeSide;

		bool leftClick = button == GLFW_MOUSE_BUTTON_LEFT;

		for (int i = 0; i < VOXEL_GRID_LENGTH;i++) {
			for (int j = 0; j < VOXEL_GRID_HEIGHT; j++) {
				for (int k = 0; k < VOXEL_GRID_DEPTH; k++) {

					int x =  i, y = VOXEL_GRID_HEIGHT - 1 - j , z = k;

					bb.xMin = glm::vec3(x, y, z).x;
					bb.xMax = glm::vec3(x + VOXEL_BOX_DIM_SIZE, y, z).x;
					bb.yMin = glm::vec3(x, y, z).y;
					bb.yMax = glm::vec3(x, y + VOXEL_BOX_DIM_SIZE , z).y;
					bb.zMin = glm::vec3(x, y, z).z;
					bb.zMax = glm::vec3(x, y, z + VOXEL_BOX_DIM_SIZE).z;

					if (Physics::CheckRayBoxCollision(ray, bb,collPoint,cubeSide)) {						

						currDist = glm::length(ray.origin - collPoint);

						if((first || (minDist > currDist)) && ((Scene::GetVoxelAtIndex(glm::vec3(x, y, z)) != 0) )){
							minDist = currDist;
							clickIndex = glm::vec3(x, y, z);
							first = false;
							hit = true;
						}
						c++;

					}
				}
			}
		}
		if (hit) {
			if (leftClick && action == GLFW_PRESS) {
				Scene::ChangeVoxelAtIndex(clickIndex, 0);
			}
			if (!leftClick && action == GLFW_PRESS) {
				glm::vec3 resIndex;

				switch (cubeSide)
				{

				case Cube::TOP:
					resIndex = clickIndex;
					resIndex.y += 1;
					Scene::ChangeVoxelAtIndex(resIndex, 1);
					break;

				case Cube::BOTTOM:
					resIndex = clickIndex;
					resIndex.y -= 1;
					Scene::ChangeVoxelAtIndex(resIndex, 1);
					break;

				case Cube::FRONT:
					resIndex = clickIndex;
					resIndex.x -= 1;
					Scene::ChangeVoxelAtIndex(resIndex, 1);
					break;

				case Cube::BACK:
					resIndex = clickIndex;
					resIndex.x += 1;
					Scene::ChangeVoxelAtIndex(resIndex, 1);
					break;

				case Cube::RIGHT:
					resIndex = clickIndex;
					resIndex.z -= 1;
					Scene::ChangeVoxelAtIndex(resIndex, 1);
					break;

				case Cube::LEFT:
					resIndex = clickIndex;
					resIndex.z += 1;
					Scene::ChangeVoxelAtIndex(resIndex, 1);
					break;

				default:
					break;
				}





				std::cout << "rightclick";
			}


	}
	


		//glm::vec3 camPos = Renderer::GetInstance()->GetCamera().GetPosition();
		//glm::vec3 clickDir = Renderer::GetInstance()->GetCamera().GetDirection();
		//
		//Physics::Ray ray = Physics::Ray(camPos, clickDir, 1.0f);
		//int c = 0;
		//for (int i = 0; i < VOXEL_GRID_LENGTH; i++) {
		//	for (int j = 0; j < VOXEL_GRID_HEIGHT; j++) {
		//		for (int k = 0; k < VOXEL_GRID_DEPTH; k++) {
		//
		//			Physics::BoundingBox bb;
		//
		//			int x = i, y = VOXEL_GRID_HEIGHT - j, z = VOXEL_GRID_DEPTH - k;
		//
		//			bb.xMin = glm::vec3(x, y, z).x;
		//			bb.xMax = glm::vec3(x + VOXEL_BOX_DIM_SIZE, y, z).x;
		//			bb.yMin = glm::vec3(x, y - VOXEL_BOX_DIM_SIZE, z).y;
		//			bb.yMax = glm::vec3(x, y, z).y;
		//			bb.zMin = glm::vec3(x, y, z).z;
		//			bb.zMax = glm::vec3(x, y, z + VOXEL_BOX_DIM_SIZE).z;
		//			glm::vec3 collPoint;
		//			if (Physics::CheckRayBoxCollision(ray, bb,collPoint)) {
		//				std::cout << "hit at: " << i << " " << j << " " << k << std::endl;
		//				Scene::ChangeVoxelAtIndex(glm::vec3(x, y - 1, z - 1), 1);
		//				GameObject posMarker;
		//				posMarker.position = glm::vec3(i, j, k);
		//				posMarker.modelId = MARKER_SPHERE;
		//				GameObjectManager::GetInstance()->AppendGameObjectToQueue(posMarker);
		//				c++;
		//				//return;
		//			}
		//		}
		//	}
		//}
		//std::cout << "hit :" << c << std::endl;

	//}

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