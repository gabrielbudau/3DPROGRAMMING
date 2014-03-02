#include "Camera.h"


Camera::Camera() : 
	viewDirection(0.0f, 0.0f, -1.0f), 
	UP(0.0f, 1.0f, 0.0f), 
	mouseRotationScale(0.5f),
	movementSpeed(0.001f){}

void Camera::mouseUpdate(const glm::vec2& newMousePosition)
{
	glm::vec2 mouseDelta = newMousePosition - oldMousePosition;
	if (glm::length(mouseDelta) > 50.f)
	{
		oldMousePosition = newMousePosition;
		return;
	}
	strafeDirection = glm::cross(viewDirection, UP);
	glm::mat4 rotator =
		glm::rotate(-mouseDelta.x * mouseRotationScale, UP) *
		glm::rotate(-mouseDelta.y * mouseRotationScale, strafeDirection);
	// rotation
	viewDirection = glm::mat3(rotator) * viewDirection;

	oldMousePosition = newMousePosition;
}


glm::mat4 Camera::getWorldToViewMatrix() const
{
	return glm::lookAt(position, position + viewDirection, UP);
}

void Camera::moveUp()
{
	position += movementSpeed * UP;
}
void Camera::moveDown()
{
	position += -movementSpeed * UP;
}
void Camera::moveForward()
{
	position += movementSpeed * viewDirection;
}
void Camera::moveBackward()
{
	position += -movementSpeed * viewDirection;
}
void Camera::strafeLeft()
{
	position += -movementSpeed * strafeDirection;
}
void Camera::strafeRight()
{
	position += movementSpeed * strafeDirection;
}

void Camera::setPosition(glm::vec3 _v){
	position = _v;
}

void Camera::setViewDirection(glm::vec3 _v){
	viewDirection = _v;
}