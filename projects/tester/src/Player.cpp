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