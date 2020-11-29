#pragma once

#include "Graphics/VertexArrayObject.h"
#include "Gameplay/Transform.h"
#include <Graphics\Shader.h>
#include <entt\entt.hpp>
#include <GLFW\glfw3.h>
#include <Gameplay\Camera.h>
#include "Graphics/Texture2D.h"
class Player
{
public:
	Player(std::string MM,
		glm::vec3 location,
		glm::vec3 rotation,
		glm::vec3 scale,
		std::string melonUV
		);

	void Render(Camera::sptr cam);

	

	entt::entity player;
	//glm::vec3 pos;
	//float x;
	//float y;
	//float z;
	Transform::sptr melonTrans = nullptr;
	VertexArrayObject::sptr melonMod = nullptr;
	Shader::sptr melonShader = nullptr;

	Texture2D::sptr melonTex = nullptr;


	//control
	void glWindow(GLFWwindow* inWindow);

	Transform::sptr control(Transform::sptr trans, float dt);
	Transform::sptr phyUpdate(Transform::sptr trans, float dt);
	void mapping();

	GLFWwindow* window;
	glm::vec3 pos;
	float groundHight = 0.0f;
	bool isGround = true;

	//test
	float acceleration = -50.0f;
	float position = 0.0f;
protected:

};

