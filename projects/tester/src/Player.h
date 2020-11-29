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
	glm::vec3 pos;
	float x;
	float y;
	float z;
	Transform::sptr melonTrans = nullptr;
	VertexArrayObject::sptr melonMod = nullptr;
	Shader::sptr melonShader = nullptr;

	Texture2D::sptr melonTex = nullptr;
};

