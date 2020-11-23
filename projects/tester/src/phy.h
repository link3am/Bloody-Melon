#pragma once
#include "iostream"
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include "Gameplay/Transform.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class phy
{
public:

	phy()
	{
	};

	


	void glWindow(GLFWwindow* inwindow)
	{
		window = inwindow;
	}

	Transform::sptr control(Transform::sptr trans, float dt)
	{
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

			trans->MoveLocalFixed(0.1f, 0.0f, 0.0f);
			trans->SetLocalRotation(0.0f, -10.0f, 0.0f);
			trans->SetLocalScale(1.0f, 1.0f, -1.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {

			trans->MoveLocalFixed(-0.1f, 0.0f, 0.0f);
			trans->SetLocalRotation(0.0f, 10.0f, 0.0f);
			trans->SetLocalScale(-1.0f, 1.0f, -1.0f);

		}
		
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {

			trans->MoveLocalFixed(0.0f, 1.0f, 0.0f);
		}
		return trans;
	}
	
	Transform::sptr phyUpdate(Transform::sptr trans, float dt)
	{
		pos = trans->GetLocalPosition();
		trans = control(trans, dt);
		if(pos.y>0)//gravity
			trans->MoveLocalFixed(0.0f, G , 0.0f);

		return trans;
	}
	

protected:
	float G = -9.8f/60;
	GLFWwindow* window;
	glm::vec3 pos;
	glm::vec2 AA;
	glm::vec2 BB;
};