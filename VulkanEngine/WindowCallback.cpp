#include "WindowCallback.h"
#include "Renderer.h"
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	// if (NO_CAM_MOVEMENT)
	// 	return;
	
	Renderer* renderRef = Renderer::GetInstance();
	
	glm::vec3 direction = renderRef->GetCamera().GetDirection();
	
	//renderRef->GetCamera().rawXposMouse = xpos;
	//renderRef->GetCamera().rawYposMouse = ypos;
	
	direction.x = cos(glm::radians(xpos)) * cos(glm::radians(-ypos));
	direction.y = sin(glm::radians(ypos));
	direction.z = sin(glm::radians(xpos)) * cos(glm::radians(ypos));
	
	renderRef->GetCamera().SetDirection(glm::normalize(direction));
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//Renderer::GetInstance()->GetCamera()->SetZoom(Renderer::GetInstance()->GetCamera()->GetZoom() - (float)yoffset);
	//if (Renderer::GetInstance()->GetCamera()->GetZoom() < 1.0f)
	//	Renderer::GetInstance()->GetCamera()->SetZoom(1.0f);
	//if (Renderer::GetInstance()->GetCamera()->GetZoom() > 45.0f)
	//	Renderer::GetInstance()->GetCamera()->SetZoom(45.0f);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	//if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
	//
	//
	//	Camera* camera = Renderer::GetInstance()->GetCamera();
	//	glm::vec3 viewPos = camera->GetPosition();
	//	float M_PI = 3.14159265359;
	//	float x = camera->rawXposMouse, y = camera->rawYposMouse;
	//
	//	float s = x / (camera->imageWidth - 1);
	//
	//	float t = 1 - y / (camera->imageHeight - 1);
	//
	//	glm::vec3 direction = camera->lower_left_corner + s * camera->horizontal + t * camera->vertical - viewPos;

		/*
		double t_max = 100.0;
		bool hit_anything = false;
		double closest_so_far = t_max;
		auto sphereQueue = DrawObjectManager::GetSphereQueue();
		int size = sphereQueue.size();


		for (int i = 0; i < sphereQueue.size(); i++) {
			Sphere currSphere = sphereQueue[i];
			if (CollisionDetection::RaySphereIntersection(currSphere.position, currSphere.radius, viewPos, direction, 0.0, closest_so_far)) {
				std::cout << "hit Sphere: " << currSphere.ID << std::endl;
				ObjSelect obj;
				obj.objectID = currSphere.ID;
				obj.objectType = SPHERE;
				Player::SetSelectedObject(obj);

			}
		}
		auto list = GameObjectManager::GetGameObjectList();
		auto queue = DrawObjectManager::GetObjectSlotQueue();


		for (int i = 0; i < list.size(); i++) {
			glm::vec3 intersectPoint;
			auto drawObj = DrawObjectManager::GetDrawObjectByID(list[i].GetDrawObjectID());
			glm::vec3 spherePos = list[i].GetTranslation() * glm::vec4(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
			if (!CollisionDetection::RaySphereIntersectionSimple(spherePos, drawObj.shapeData.maxRad, viewPos, direction)) break;
			for (auto& mesh : drawObj.shapeData.meshVerts) {
				mesh[0] = list[i].GetTranslation() * glm::vec4(mesh[0], 1.0f);
				mesh[1] = list[i].GetTranslation() * glm::vec4(mesh[1], 1.0f);
				mesh[2] = list[i].GetTranslation() * glm::vec4(mesh[2], 1.0f);
				if (CollisionDetection::RayTriangleIntersect(viewPos, direction, 0.0f, closest_so_far, mesh, intersectPoint)) {
					std::cout << "hit at obj: " << i << "at point: " << glm::to_string(intersectPoint) << std::endl;
					ObjSelect obj;
					obj.objectID = i;
					obj.objectType = TRIANGLE;
					Player::SetSelectedObject(obj);

				}
			}

		}*/


	// }
	// if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
	// 
	// 
	// }

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	auto camRef = Renderer::GetInstance()->GetCamera();
	auto playerPos = camRef.GetPosition();

	const float cameraSpeed = 0.05f; // adjust accordingly
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key == GLFW_KEY_W && action != GLFW_RELEASE) {

		playerPos.z -= .05f;
		Renderer::GetInstance()->GetCamera().SetPosition(playerPos);

	}
	if (key == GLFW_KEY_S && action != GLFW_RELEASE) {

		playerPos.z += .05f;
		Renderer::GetInstance()->GetCamera().SetPosition(playerPos);
	}

	if (key == GLFW_KEY_A && action != GLFW_RELEASE) {

		playerPos.x -= .05f;
		Renderer::GetInstance()->GetCamera().SetPosition(playerPos);

	}
	if (key == GLFW_KEY_D && action != GLFW_RELEASE) {

		playerPos.x += .05f;
		Renderer::GetInstance()->GetCamera().SetPosition(playerPos);
	}
	if (key == GLFW_KEY_SPACE && action != GLFW_RELEASE) {

		playerPos.y += .05f;
		Renderer::GetInstance()->GetCamera().SetPosition(playerPos);
	}
	if (key == GLFW_KEY_LEFT_ALT && action != GLFW_RELEASE) {

		playerPos.y -= .05f;
		Renderer::GetInstance()->GetCamera().SetPosition(playerPos);
	}
}