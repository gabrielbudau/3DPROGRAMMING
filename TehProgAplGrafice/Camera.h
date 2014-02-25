#pragma once
#include <glm.hpp>
#include<gtx\transform.hpp>
class Camera
{
	glm::vec3 position;
	glm::vec3 viewDirection;
	const glm::vec3 UP;
	glm::vec2 oldMousePosition;
	float mouseRotationScale;
	float movementSpeed;
	glm::vec3 strafeDirection;
public:
	Camera();
	void mouseUpdate(const glm::vec2& newMousePosition);
	void moveUp();
	void moveDown();
	void moveForward();
	void moveBackward();
	void strafeLeft();
	void strafeRight();
	glm::mat4 getWorldToViewMatrix() const;
	void setPosition(glm::vec3 _v);
	void setViewDirection(glm::vec3 _v);
};

