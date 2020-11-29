#include "Player.h"
#include <Graphics\ObjLoader.h>
#include <Graphics\Texture2DData.h>
#include <Graphics\Texture2D.h>

Player::Player(std::string MM,
	glm::vec3 location,
	glm::vec3 rotation,
	glm::vec3 scale,
	std::string melonUV
) {
	melonMod = ObjLoader::LoadFromFile(MM);
	melonTrans = Transform::Create();
	melonTrans->Transform::SetLocalPosition(location);
	melonTrans->Transform::SetLocalRotation(rotation);
	melonTrans->Transform::SetLocalScale(scale);



	melonShader = Shader::Create();
	melonShader->LoadShaderPartFromFile("shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
	melonShader->LoadShaderPartFromFile("shaders/frag_blinn_phong_textured.glsl", GL_FRAGMENT_SHADER);
	melonShader->Link();

	melonTex = Texture2D::LoadFromFile(melonUV);
}


void Player::Render(Camera::sptr cam) {
	melonShader->Bind();
	melonShader->SetUniform("s_tex", 2);
	melonTex->Bind(2);
	
	melonShader->SetUniformMatrix("u_ModelViewProjection", cam->GetViewProjection() * melonTrans->LocalTransform());
	melonShader->SetUniformMatrix("u_Model", melonTrans->LocalTransform());
	melonMod->Render();
}


///////3am 
void Player::glWindow(GLFWwindow* inWindow)
{
	window = inWindow;
}
Transform::sptr Player::control(Transform::sptr trans, float dt)
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
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		if (isGround == true && pos.y >= 0.2)
		{
			trans->MoveLocalFixed(0.0f, -0.1f, 0.0f);
			isGround = false;
			groundHight = 0.0f;
		}
	}
	return trans;
}
Transform::sptr Player::phyUpdate(Transform::sptr trans, float dt)
{
	mapping();

	trans = control(trans, dt);
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
void Player::mapping()
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
