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
	




		GameObject posMarker;
		


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

		glm::vec3 camPos = Renderer::GetInstance()->GetCamera().GetPosition();
		glm::vec3 clickDir = Renderer::GetInstance()->GetCamera().GetDirection();
		Physics::Ray ray = Physics::Ray(camPos, clickDir, 1.0f);

		int camX = camPos.x > 0 ? camPos.x : 0,
			camY = 1,// camPos.y > 0 ? camPos.y / chunkManagerRef->GetHeigth() : 0,
			camZ = camPos.z > 0 ? camPos.z  : 0;
		Chunk currentChunk = chunkManagerRef->GetChunkFromChunkArr(glm::vec3(camX / CHUNK_LENGTH, 0, camZ / CHUNK_DEPTH));
		std::vector<Chunk> chunkArr;
		chunkArr.reserve(3);
		glm::vec3 normClickDir = normalize(clickDir);
		ray.gradient = Physics::CalculateGradient(ray.origin.x,ray.origin.y,ray.origin.x+ray.direction.x,ray.origin.y+ray.direction.y);


		//int x0 = camX;
		//int x1 = camX + normClickDir.x * CHUNK_LENGTH > (CHUNK_LENGTH - 1) ? CHUNK_LENGTH - 1 : camX + normClickDir.x * CHUNK_LENGTH;
		//int y0 = camZ;
		//int y1 = camZ + normClickDir.z * CHUNK_DEPTH > (CHUNK_DEPTH - 1) ? CHUNK_DEPTH - 1 : camZ + normClickDir.z * CHUNK_DEPTH;
		//int e2 = 0;
		//
		//int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
		//int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
		//
		////int dx = abs(x1 - x0), sx = x0 < x1 ? CHUNK_LENGTH : -CHUNK_LENGTH;
		////int dy = -abs(y1 - y0), sy = y0 < y1 ? CHUNK_DEPTH : -CHUNK_DEPTH;
		//
		//int err = dx + dy; // Fehlerwert e_xy
		//
		//posMarker.position = glm::vec3(x0, 0, y0);
		//posMarker.modelId = MARKER_SPHERE;
		//GameObjectManager::GetInstance()->AppendGameObjectToQueue(posMarker);
		//
		//posMarker.position = glm::vec3(x1, 0, y1);
		//posMarker.modelId = MARKER_SPHERE;
		//GameObjectManager::GetInstance()->AppendGameObjectToQueue(posMarker);
		//
		//std::cout << "begin x0: " << x0 << " begin y0: " << y0 << std::endl;
		//std::cout << "end x1: " << x1 << " end y1: " << y1 << std::endl;
		//
		//while (true) {
		//
		//
		//
		//	std::cout << "x0: " << x0 << " y0: " << y0 << std::endl;
		//
		//	// Positionen für Chunks berechnen
		//	int posX = x0 / CHUNK_LENGTH;
		//	int posY = y0 / CHUNK_DEPTH;
		//
		//	if (posX < 0) posX = 0;
		//	if (!(posX < chunkManagerRef->GetLength() * CHUNK_LENGTH)) posX = chunkManagerRef->GetLength() * CHUNK_LENGTH - 1;
		//
		//	if (posY < 0) posY = 0;
		//	if (!(posY < chunkManagerRef->GetDepth() * CHUNK_DEPTH)) posY = chunkManagerRef->GetDepth() * CHUNK_DEPTH - 1;
		//
		//	chunkArr.push_back(chunkManagerRef->GetChunkFromChunkArr(glm::vec3(posX, 0, posY)));
		//
		//	posMarker.position = glm::vec3(x0, 0, y0);
		//	posMarker.modelId = MARKER_SPHERE;
		//	GameObjectManager::GetInstance()->AppendGameObjectToQueue(posMarker);
		//
		//	if (x0 >= x1 && y0 >= y1) {
		//		break;
		//	}
		//
		//	e2 = 2 * err;
		//
		//	if (e2 > dy) {
		//		err += dy;
		//		x0 += sx * CHUNK_LENGTH;
		//		//x0 += sx ;
		//	}
		//
		//	if (e2 < dx) {
		//		err += dx;
		//		y0 += sy * CHUNK_DEPTH;
		//	}
		//
		//	// Sicherstellen, dass y0 und y1 innerhalb der Grenzen bleiben
		//
		//}


		int x0 = camX;
		int x1 = camX + normClickDir.x * CHUNK_LENGTH > (CHUNK_LENGTH - 1) ? CHUNK_LENGTH - 1 : camX + normClickDir.x * CHUNK_LENGTH;
		int y0 = camZ;
		int y1 = camZ + normClickDir.z * CHUNK_DEPTH > (CHUNK_DEPTH - 1) ? CHUNK_DEPTH - 1 : camZ + normClickDir.z * CHUNK_DEPTH;

		int dx = abs(x1 - x0);
		int dy = abs(y1 - y0);
		int sx = x0 < x1 ? CHUNK_LENGTH : -CHUNK_LENGTH;
		int sy = y0 < y1 ? CHUNK_DEPTH : -CHUNK_DEPTH;

		bool steep = dy > dx;
		if (steep) {
			std::swap(x0, y0);
			std::swap(x1, y1);
			std::swap(dx, dy);
			std::swap(sx, sy);
		}

		float gradient = static_cast<float>(dy) / dx;
		float intery = y0 + (gradient > 1 ? 0.5 : 0);  // Interpolated y coordinate (initially y0)
		int xend = x0;

		for (int x = x0; x < x1 + sx; x += sx) {
			if (steep) {
				// Plot at (y, x) if the line is steep
				if (intery >= 0 && intery < chunkManagerRef->GetDepth() * CHUNK_DEPTH &&
					x >= 0 && x < chunkManagerRef->GetLength() * CHUNK_LENGTH) {
					posMarker.position = glm::vec3(intery, 0, x);
					posMarker.modelId = MARKER_SPHERE;
					GameObjectManager::GetInstance()->AppendGameObjectToQueue(posMarker);
				}
			}
			else {
				// Plot at (x, y) if the line is not steep
				if (x >= 0 && x < chunkManagerRef->GetLength() * CHUNK_LENGTH &&
					intery >= 0 && intery < chunkManagerRef->GetDepth() * CHUNK_DEPTH) {
					posMarker.position = glm::vec3(x, 0, intery);
					posMarker.modelId = MARKER_SPHERE;
					GameObjectManager::GetInstance()->AppendGameObjectToQueue(posMarker);
				}
			}

			// Apply antialiasing by plotting additional pixels next to the current
			float coverage = fabs(intery - floor(intery));
			if (x >= 0 && x < chunkManagerRef->GetLength() * CHUNK_LENGTH) {
				// Plot below
				if (floor(intery) >= 0 && floor(intery) < chunkManagerRef->GetDepth() * CHUNK_DEPTH) {
					posMarker.position = glm::vec3(x, 0, floor(intery));
					posMarker.modelId = MARKER_SPHERE; // Adjust based on your brightness
					GameObjectManager::GetInstance()->AppendGameObjectToQueue(posMarker);
				}
				// Plot above
				if (floor(intery) + 1 >= 0 && floor(intery) + 1 < chunkManagerRef->GetDepth() * CHUNK_DEPTH) {
					posMarker.position = glm::vec3(x, 0, floor(intery) + 1);
					posMarker.modelId = MARKER_SPHERE; // Adjust based on your brightness
					GameObjectManager::GetInstance()->AppendGameObjectToQueue(posMarker);
				}
			}

			// Sicherstellen, dass posX und posY innerhalb der Grenzen bleiben
			int posX = x / CHUNK_LENGTH;
			int posY = intery / CHUNK_DEPTH;


			
			if (posX < 0) posX = 0;
			if ((posX >= chunkManagerRef->GetLength())) posX = chunkManagerRef->GetLength() - 1;
			
			if (posY < 0) posY = 0;
			if ((posY >= chunkManagerRef->GetDepth())) posY = chunkManagerRef->GetDepth() - 1;

			chunkArr.push_back(chunkManagerRef->GetChunkFromChunkArr(glm::vec3(posX, 0, posY)));

			intery += gradient * CHUNK_DEPTH;  // Increment y-coordinate for the next step
		}




		bool leftClick = button == GLFW_MOUSE_BUTTON_LEFT;

		if (action == GLFW_RELEASE) {



			chunkArr.push_back(currentChunk);
			//for (int chunkNum = 0; chunkNum < chunkManagerRef->GetChunkArrSize(); chunkNum++) {
			for (Chunk& chunk: chunkArr) {
				//Chunk& chunk = chunkManagerRef->GetChunkFromChunkArr(chunkNum);

				posMarker.position = glm::vec3(chunk.position.x * CHUNK_LENGTH,0, chunk.position.z * CHUNK_DEPTH);
				posMarker.modelId = MARKER_SPHERE;
				//GameObjectManager::GetInstance()->AppendGameObjectToQueue(posMarker);

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