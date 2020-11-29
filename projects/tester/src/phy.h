#pragma once
#include "iostream"
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include "Gameplay/Transform.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class Phy
{
public:

	Phy(int x) {};


	Transform::sptr phyUpdate(Transform::sptr trans, float dt)
	{
		mapping();
		
		//trans = control(trans, dt);
		pos = trans->GetLocalPosition();
		//test
		
		if (pos.y < groundHight)
		{
			pos = trans->GetLocalPosition();
			trans->SetLocalPosition(pos.x, groundHight, pos.z);
			isGround = true;
			//test
			position = 0.0f;
		}
		if (pos.y > groundHight)//gravity
		{
			//test 
			position += acceleration * dt * dt;

		}
	
		trans->MoveLocalFixed(0.0f, position, 0.0f);
		
	
		std::cout << pos.y << std::endl;
		return trans;
	}
	void mapping()
	{
		if (pos.x > 5.0f && pos.x < 7.0f && pos.y >= 2.0f)
		{
			groundHight = 2.0f;
			
		}
		else if (pos.x > 3.0f && pos.x < 5.0f && pos.y >= 1.0f)
		{
			groundHight = 1.0f;
		}
		else if (pos.x > 11.0f && pos.x < 13.0f && pos.y >= 4.0f)
		{
			groundHight = 4.0f;
		
		}
		else if (pos.x > 17.0f && pos.x < 19.0f && pos.y >= 5.0f)
		{
			groundHight = 5.0f;
			
		}
		else
		{
			groundHight = 0;
		}
	
	}

protected:
	glm::vec3 pos;
	float groundHight = 0.0f;
	bool isGround = true;

	//test
	float acceleration = -50.0f;
	float position = 0.0f; 
};