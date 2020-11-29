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
		
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

			if (isGround == true) {
				position += 15 * dt;
				isGround = false;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			if (isGround == true && pos.y >=0.2) 
			trans->MoveLocalFixed(0.0f, -0.1f, 0.0f);
			isGround = false;
			}
		return trans;
	}
	
	Transform::sptr phyUpdate(Transform::sptr trans, float dt)
	{
		mapping();
		pos = trans->GetLocalPosition();
		trans = control(trans, dt);
		//test
		

		if (pos.y > groundHight && isGround==false)//gravity
		{
			//test 
			position += acceleration * dt*dt;
			
			//end
			pos = trans->GetLocalPosition();
			
		}
		if (pos.y < groundHight)
		{
			pos = trans->GetLocalPosition();
			trans->SetLocalPosition(pos.x, groundHight, pos.z);
			isGround = true;
			//test
			position = 0.0f;
			velocity = 0.0;
		}
		trans->MoveLocalFixed(0.0f, position, 0.0f);
		
	
		return trans;
	}
	void mapping()
	{
		if (pos.x > 5.0f && pos.x < 7.0f && pos.y >= 2)
		{
			groundHight = 2;
		}
		else if (pos.x > 3.0f && pos.x < 5.0f && pos.y >= 1)
		{
			groundHight = 1;
		}
		else if (pos.x > 11.0f && pos.x < 13.0f && pos.y >= 4)
		{
			groundHight = 4;
		}
		else if (pos.x > 17.0f && pos.x < 19.0f && pos.y >= 5)
		{
			groundHight = 5;
		}
		else 
		{
			groundHight = 0;
		}
		
	}

protected:
	GLFWwindow* window;
	glm::vec3 pos;
	float groundHight = 0.0f;
	bool isGround = true;

	//test
	float acceleration = -50.0f;
	float velocity = 0.0;
	float position = 0.0f; 
};