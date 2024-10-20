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

		//GameObject posMarker;
		//
		//posMarker.position = ray.origin + ray.direction;
		//posMarker.modelId = MARKER_SPHERE;
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
		ChunkManager* chunkManagerRef = ChunkManager::GetInstance();
		glm::vec3 hitChunkPos;

		bool leftClick = button == GLFW_MOUSE_BUTTON_LEFT;

		if (action == GLFW_RELEASE) {
			
			for (int chunkNum = 0; chunkNum < chunkManagerRef->GetChunkArrSize(); chunkNum++) {
				Chunk& chunk = chunkManagerRef->GetChunkFromChunkArr(chunkNum);

				for (int i = 0; i < chunk.length; i++) {
					for (int j = 0; j < chunk.height; j++) {
						for (int k = 0; k < chunk.depth; k++) {

							size_t x = (size_t)i,
								y = (size_t)(chunk.height - 1) - j,
								z = (size_t)k;

							size_t x_room = (size_t)chunk.length * chunk.position.x + (size_t)i,
								y_room = (size_t)chunk.height * chunk.position.y + (size_t)(chunk.height - 1) - j,
								z_room = (size_t)chunk.depth * chunk.position.z + (size_t)k;

							bb.xMin = glm::vec3(x_room, y_room, z_room).x;
							bb.xMax = glm::vec3(x_room + VOXEL_BOX_DIM_SIZE, y_room, z_room).x;
							bb.yMin = glm::vec3(x_room, y_room, z_room).y;
							bb.yMax = glm::vec3(x_room, y_room + VOXEL_BOX_DIM_SIZE, z_room).y;
							bb.zMin = glm::vec3(x_room, y_room, z_room).z;
							bb.zMax = glm::vec3(x_room, y_room, z_room + VOXEL_BOX_DIM_SIZE).z;

							glm::vec3 origVec = ray.origin - glm::vec3(x_room, y_room, z_room);

							float dotProduct = glm::dot(glm::normalize(ray.direction), origVec);

							// skip block if it's behind the camera
							if (!(dotProduct <= 0))
								continue;

							if (Physics::CheckRayBoxCollision(ray, bb, collPoint, cubeSide)) {

								currDist = glm::distance(ray.origin, collPoint);

								if ((first || (minDist > currDist)) && ((Scene::GetVoxelAtIndex(chunk, glm::vec3(x, y, z)) != 0))) {
									finalCollPoint = collPoint;
									minDist = currDist;
									clickIndex = glm::vec3(x, y, z);
									first = false;
									hit = true;
									closestCubeSide = cubeSide;
									hitChunkPos = chunk.position;
								}

							}
						}
					}
				}
			
			}
				if (hit) {
					//posMarker.position = finalCollPoint;
					//GameObjectManager::GetInstance()->AppendGameObjectToQueue(posMarker);
					if (leftClick) {
						Scene::ChangeVoxelAtIndex(chunkManagerRef->GetChunkFromChunkArr(hitChunkPos), clickIndex, 0);
					}
					if (!leftClick) {
						
						Scene::voxelMesh.AddCubeToCubeSide(chunkManagerRef->GetChunkFromChunkArr(hitChunkPos), clickIndex, closestCubeSide);
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