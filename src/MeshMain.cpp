#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <stb_image/stb_image.h>

#include <string>
#include <iostream>
#include <vector>
#include <functional>
#include <format>
#include <math.h>

#include "Camera.hpp"
#include "ShaderProgram.hpp"
#include "Application.hpp"
#include "FileReader.hpp"
#include "Texture.hpp"
#include "Types.hpp"
#include "scene/Model.hpp"
#include "ecs/ECS.hpp"


void proccessInput() {

	auto& context = dlb::ApplicationSingleton::getInstance();
	auto window = context.getWindow();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}
		else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {

			context.getWireframeMode() ?
				glPolygonMode(GL_FRONT_AND_BACK, GL_POLYGON) :
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			context.setWireframeMode(!context.getWireframeMode());
		}
		else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
			//std::cout << "[PAUSED]: " << context.paused << std::endl;
		}
		else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			context.getCamera().goForward();
		}
		else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			context.getCamera().goLeft();
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			context.getCamera().goBackwards();
		}
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			context.getCamera().goRight();
		} //else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			//context.position.y -= 0.1f;
		//}
}

void ImGuiPanelRendering(ecs::EntityPool& ep) {
	auto& context = dlb::ApplicationSingleton::getInstance();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(300, 0));
	ImGui::Begin("Scene Information");

	ImGui::Text(std::format("FPS: {}/{}",
		context.getFrames(), 1000.0f / (float)context.getFrames()).c_str());

	ImGui::ColorEdit3("Bg Color", glm::value_ptr(context.getBgColor())); // Adjust light source color

	ImGui::Text(std::format("X: {} Y: {} Z: {}",
		context.getCamera().getPosition().x, context.getCamera().getPosition().y, context.getCamera().getPosition().z).c_str());

	ImGui::Text(std::format("Looking at X: {} Y: {} Z: {}",
		context.getCamera().getDirection().x, context.getCamera().getDirection().y, context.getCamera().getDirection().z).c_str());

	ImGui::InputFloat("Camera Speed", &context.getCamera().getCameraSpeedRef());

	ImGui::Checkbox("Pause ECS", &context.getPauseEcs());

	ImGui::Text("Global Light");
	ImGui::InputFloat3("Light Direction", glm::value_ptr(context.getGlobalLight().direction), "%.2f");
	ImGui::ColorEdit3("Ambient", glm::value_ptr(context.getGlobalLight().ambient));
	ImGui::ColorEdit3("Diffuse", glm::value_ptr(context.getGlobalLight().diffuse));
	ImGui::ColorEdit3("Specular", glm::value_ptr(context.getGlobalLight().specular));

	for (const auto& entity : ep.getEntities()) {
		auto& model = context.getModel(entity.model_id);
		auto& aabb = model.getAABB();
		ImGui::Text("Entity %d", entity.id);
		ImGui::Text("MIN %.3f, %.3f %.3f", aabb.min.x, aabb.min.y, aabb.max.z);
		ImGui::Text("MAX %.3f, %.3f %.3f", aabb.max.x, aabb.max.y, aabb.max.z);
	}

	ImGui::End();
}

int main() {
	auto& context = dlb::ApplicationSingleton::getInstance();
	auto window = context.getWindow();

	stbi_set_flip_vertically_on_load(true);

	// Initialize ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	auto& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

#pragma region Shader programs
	auto textured_model_shader = context.addShader(
		dlb::ShaderProgramBuilder{}
		.vertexShader("C:\\Users\\Diego\\Documents\\Code\\LearnOpenGL\\resources\\shaders\\ModelWithTextures.vert")
		.fragmentShader("C:\\Users\\Diego\\Documents\\Code\\LearnOpenGL\\resources\\shaders\\ModelWithTextures.frag"));

	auto material_model_shader = context.addShader(
		dlb::ShaderProgramBuilder{}
		.vertexShader("C:\\Users\\Diego\\Documents\\Code\\LearnOpenGL\\resources\\shaders\\ModelWithMaterials.vert")
		.fragmentShader("C:\\Users\\Diego\\Documents\\Code\\LearnOpenGL\\resources\\shaders\\ModelWithMaterials.frag"));

	auto notextures_model_shader = context.addShader(
		dlb::ShaderProgramBuilder{}
			.vertexShader("C:\\Users\\Diego\\Documents\\Code\\LearnOpenGL\\resources\\shaders\\ModelWithMaterials.vert")
			.fragmentShader("C:\\Users\\Diego\\Documents\\Code\\LearnOpenGL\\resources\\shaders\\ModelWithout.frag"));
#pragma endregion

#pragma region Scene Configuration
	auto tree_model = context.addModel("C:\\Users\\Diego\\Documents\\Code\\LearnOpenGL\\resources\\models\\low_poly_tree\\Lowpoly_tree_sample.obj", scene::ModelFlags::UseMaterials | scene::ModelFlags::DrawAABB);
	auto wheel5 = context.addModel("C:\\Users\\Diego\\Documents\\Code\\LearnOpenGL\\resources\\models\\5wheel\\wheel5.obj", scene::ModelFlags::UseMaterials | scene::ModelFlags::DrawAABB);

	ecs::EntityPool entity_pool{};

	entity_pool.newEntity(
		glm::vec3(-10.0f, 0.0f, 20.0f),
		glm::vec3(0.0f, 0.0f, -0.5f),
		glm::vec3(0),
		notextures_model_shader,
		wheel5
	);

	entity_pool.newEntity(
		glm::vec3(-10.0f, 0.0f, -20.0f),
		glm::vec3(0.0f, 0.0f, .5f),
		glm::vec3(0),
		notextures_model_shader,
		wheel5
	);
#pragma endregion

	while (!glfwWindowShouldClose(window)) {
		proccessInput();
		context.updateTime();
		const auto& bg_color = context.getBgColor();
		glClearColor(bg_color.r, bg_color.g, bg_color.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#pragma region ImGui Rendering
		ImGuiPanelRendering(entity_pool);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#pragma endregion

		entity_pool.iterate();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}

