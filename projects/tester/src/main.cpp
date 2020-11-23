#include <Logging.h>
#include <iostream>
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//
#include <filesystem>
#include <json.hpp>
#include <fstream>
//
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
//entity
#include "entt.hpp"
#include "phy.h"
//obj loader from TU
#include "Gameplay/Camera.h"
#include "Graphics/IBuffer.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/VertexArrayObject.h"
#include "Graphics/Shader.h"
#include "Gameplay/Transform.h"
#include "Graphics/MeshBuilder.h"
#include "Graphics/ObjLoader.h"
#include "Graphics/VertexTypes.h"
//textrue
#include "Graphics/Texture2D.h"
#include "Graphics/Texture2DData.h"
//phy
//#include <bullet/btBulletDynamicsCommon.h>
void GlDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	std::string sourceTxt;
	switch (source) {
	case GL_DEBUG_SOURCE_API: sourceTxt = "DEBUG"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: sourceTxt = "WINDOW"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceTxt = "SHADER"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY: sourceTxt = "THIRD PARTY"; break;
	case GL_DEBUG_SOURCE_APPLICATION: sourceTxt = "APP"; break;
	case GL_DEBUG_SOURCE_OTHER: default: sourceTxt = "OTHER"; break;
	}
	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:          LOG_INFO("[{}] {}", sourceTxt, message); break;
	case GL_DEBUG_SEVERITY_MEDIUM:       LOG_WARN("[{}] {}", sourceTxt, message); break;
	case GL_DEBUG_SEVERITY_HIGH:         LOG_ERROR("[{}] {}", sourceTxt, message); break;
#ifdef LOG_GL_NOTIFICATIONS
	case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_INFO("[{}] {}", sourceTxt, message); break;
#endif
	default: break;
	}
}

GLFWwindow* window;
Camera::sptr camera = nullptr;
void GlfwWindowResizedCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	//camera->ResizeWindow(width, height);
}

bool initGLFW() {
	if (glfwInit() == GLFW_FALSE) {
		LOG_ERROR("Failed to initialize GLFW");
		return false;
	}

#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

	//Create a new GLFW window
	window = glfwCreateWindow(800, 800, "TT", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set our window resized callback
	glfwSetWindowSizeCallback(window, GlfwWindowResizedCallback);

	return true;
}

bool initGLAD()
{
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
		LOG_ERROR("Failed to initialize Glad");
		return false;
	}
	return true;
}


void RenderVAO(const Shader::sptr& shader, const VertexArrayObject::sptr& vao, const Camera::sptr& camera, const Transform::sptr& transform)
{
	shader->SetUniformMatrix("u_ModelViewProjection", camera->GetViewProjection() * transform->LocalTransform());
	shader->SetUniformMatrix("u_Model", transform->LocalTransform());
	vao->Render();
}

//conrtol
/*
Transform::sptr control(Transform::sptr trans,float dt)
{
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

		trans->MoveLocalFixed(5.0f * dt, 0.0f, 0.0f);
		trans->SetLocalRotation(0.0f, -10.0f, 0.0f);
		trans->SetLocalScale(1.0f,1.0f,-1.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		
			trans->MoveLocalFixed(-5.0f * dt, 0.0f, 0.0f);
			trans->SetLocalRotation(0.0f, 10.0f, 0.0f);
			trans->SetLocalScale(-1.0f, 1.0f, -1.0f);
		
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		
			trans->MoveLocalFixed(0.0f, 5.0f*dt, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		
			trans->MoveLocalFixed(0.0f, -5.0f * dt, 0.0f);
	}
	
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {

		trans->MoveLocalFixed(0.0f, 0.0f, -5.0f * dt);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {

		trans->MoveLocalFixed(0.0f, 0.0f, 5.0f * dt);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {

		trans->RotateLocal(60.0f * dt, 0.0f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {

		trans->RotateLocal(-60.0f * dt, 0.0f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {

		trans->RotateLocal(0.0f, -60.0f * dt, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {

		trans->RotateLocal(0.0f, 60.0f * dt, 0.0f);
	}
	return trans;
}*/



int main()
{ 
	Logger::Init(); // We'll borrow the logger from the toolkit, but we need to initialize it
	if (!initGLFW())
		return 1;
	if (!initGLAD())
		return 1;
	// Let OpenGL know that we want debug output, and route it to our handler function
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(GlDebugMessage, nullptr);
	//textrue
	glEnable(GL_TEXTURE_2D);
	// GL states
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	
	//////////////////////////////////////////mod and transform
	//melon = 1.6m
	VertexArrayObject::sptr melonMod = ObjLoader::LoadFromFile("watermelon.obj");
	Transform::sptr melonTrans = Transform::Create();
	melonTrans->Transform::SetLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	melonTrans->Transform::SetLocalRotation(glm::vec3(0.0f, -10.0f, 0.0f));
	melonTrans->Transform::SetLocalScale(glm::vec3(1.0f, 1.0f, -1.0f));
	
	//map
	VertexArrayObject::sptr mapMod = ObjLoader::LoadFromFile("mapping.obj");
	Transform::sptr mapTrans = Transform::Create();
	mapTrans->Transform::SetLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	mapTrans->Transform::SetLocalRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	mapTrans->Transform::SetLocalScale(glm::vec3(1.0f, 1.0f, 1.0f));


	//////////////////////////////////////////Load our shaders
	Shader::sptr melonShader = Shader::Create();
	melonShader->LoadShaderPartFromFile("shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
	melonShader->LoadShaderPartFromFile("shaders/frag_blinn_phong_textured.glsl", GL_FRAGMENT_SHADER);
	melonShader->Link();
	Shader::sptr mapShader = Shader::Create();
	mapShader->LoadShaderPartFromFile("shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
	mapShader->LoadShaderPartFromFile("shaders/frag_blinn_phong_textured.glsl", GL_FRAGMENT_SHADER);
	mapShader->Link();
	//////////////////////////////////////////textrue
	Texture2DData::sptr melonTexData = Texture2DData::LoadFromFile("images/melon UV.png");
	Texture2DData::sptr mapTexData = Texture2DData::LoadFromFile("images/melon UV.png");
	
	Texture2D::sptr melonTex = Texture2D::Create();
	melonTex->LoadData(melonTexData);
	Texture2D::sptr mapTex = Texture2D::Create();
	mapTex->LoadData(mapTexData);
	
	//////////////////////////////////////////camera
	camera = Camera::Create();
	glm::vec3 cameraPosition = glm::vec3(0, 3, 7);
	camera->SetPosition(cameraPosition); // Set initial position
	camera->SetUp(glm::vec3(0, 1, 0)); // Use a z-up coordinate system
	camera->LookAt(glm::vec3(0.0f, 0.0f, 0.0f)); // Look at center of the screen
	camera->SetFovDegrees(90.0f); // Set an initial FOV
	camera->SetOrthoHeight(3.0f);
	//////////////////////////////////////////ecs
	static entt::registry ecs;


	entt::entity player = ecs.create();
	entt::entity enemy1 = ecs.create();
	ecs.emplace<phy>(player);
	ecs.get<phy>(player).glWindow(window);

	//////////////////////////////////////////delta time
	double lastFrame = glfwGetTime();
	double lastFrameTime = glfwGetTime();
	double timer2 = glfwGetTime();
	//fake phy
	double i = 0.0;
	float mass = -9.8;
	float position = 0.0;
	float force = 5;
	bool isGround = true;

	if (isGround == false) 
	{
		position = (i * i * i * 9.8) + (i * i * force);

	}

	const double fpsLimit = 1.0 / 60.0;
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		//DT
		double thisFrame = glfwGetTime();
		float dt = static_cast<float>(thisFrame - lastFrame);// delta time

		glClearColor(0.08f, 0.17f, 0.31f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//shader
		melonShader->Bind();
		melonShader->SetUniform("s_tex", 2);
		melonTex->Bind(2);
		RenderVAO(melonShader, melonMod, camera, melonTrans);

		
		mapShader->Bind();
		mapShader->SetUniform("s_tex", 2);
		mapTex->Bind(2);
		
		RenderVAO(mapShader, mapMod, camera, mapTrans);
		

		//fps limit in this if()
		if ((thisFrame - lastFrameTime) >= fpsLimit)
		{
			melonTrans = ecs.get<phy>(player).phyUpdate(melonTrans,dt);
			camera->cameraMove(window);

			lastFrameTime = thisFrame;
		}
		glfwSwapBuffers(window);

		//phy
		timer2 = glfwGetTime();
		//std::cout << timer2 <<std::endl;
		lastFrame = thisFrame;
	}


	// Clean up the toolkit logger so we don't leak memory
	Logger::Uninitialize();
	return 0;
} 
