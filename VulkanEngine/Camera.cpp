#include "Camera.h"

Camera::Camera(glm::vec3 position) {
	this->position = position;
	this->target = glm::vec3(0.0f, 0.0f, 0.0f);
	this->direction = glm::vec3(0.0f, 0.0f, -1.0f);
	this->right = glm::vec3(0.0f, 0.0f, 0.0f);
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->view = glm::mat4(1.0f);
	this->zoom = 45.0f;
}

Camera::~Camera() {


}

glm::mat4 Camera::GetViewMat() {
	return this->view;
}

void Camera::SetViewMat(glm::mat4 viewMat) {
	this->view = viewMat;
}

glm::vec3 Camera::GetPosition() {
	return this->position;
}

void Camera::SetPosition(glm::vec3 position) {
	this->position = position;
}

glm::vec3 Camera::GetTarget() {
	return this->target;
}

void Camera::SetTarget(glm::vec3 target) {
	this->target = target;
}

glm::vec3 Camera::GetDirection() {
	return this->direction;
}

void Camera::SetDirection(glm::vec3 direction) {
	this->direction = direction;
}

glm::vec3 Camera::GetUp() {
	return this->up;
}

void Camera::SetUp(glm::vec3 up) {
	this->up = up;
}

float Camera::GetZoom() {
	return this->zoom;
}

void Camera::SetZoom(float zoom) {
	this->zoom = zoom;
}

glm::mat4 Camera::LookAt() {

	return glm::lookAt(this->position, this->position + this->direction, this->up);
}

float Camera::GetCameraSpeed() {
	return cameraSpeed;
}

void Camera::SetCameraSpeed(float cameraSpeed) {
	this->cameraSpeed = cameraSpeed;
}