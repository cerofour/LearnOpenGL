#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
#include "Renderable.hpp"
#include "FileReader.hpp"
#include "Texture.hpp"

std::vector<float> vertices {
#include "cube_vertices"
};

dlb::Material materials[] = {
	{ glm::vec3(0.0215f, 0.1745f, 0.0215f), glm::vec3(0.0215f, 0.1745f, 0.0215f), glm::vec3(0.633f, 0.727811f, 0.633f), 0.6f },   // emerald
	{ glm::vec3(0.135f, 0.2225f, 0.1575f), glm::vec3(0.54f, 0.89f, 0.63f), glm::vec3(0.316228f, 0.316228f, 0.316228f), 0.1f },   // jade
	{ glm::vec3(0.05375f, 0.05f, 0.06625f), glm::vec3(0.18275f, 0.17f, 0.22525f), glm::vec3(0.332741f, 0.328634f, 0.346435f), 0.3f },   // obsidian
	{ glm::vec3(0.25f, 0.20725f, 0.20725f), glm::vec3(1.0f, 0.829f, 0.829f), glm::vec3(0.296648f, 0.296648f, 0.296648f), 0.088f },   // pearl
	{ glm::vec3(0.1745f, 0.01175f, 0.01175f), glm::vec3(0.61424f, 0.04136f, 0.04136f), glm::vec3(0.727811f, 0.626959f, 0.626959f), 0.6f },   // ruby
	{ glm::vec3(0.1f, 0.18725f, 0.1745f), glm::vec3(0.396f, 0.74151f, 0.69102f), glm::vec3(0.297254f, 0.30829f, 0.306678f), 0.1f },   // turquoise
	{ glm::vec3(0.329412f, 0.223529f, 0.027451f), glm::vec3(0.780392f, 0.568627f, 0.113725f), glm::vec3(0.992157f, 0.941176f, 0.807843f), 0.21794872f },   // brass
	{ glm::vec3(0.2125f, 0.1275f, 0.054f), glm::vec3(0.714f, 0.4284f, 0.18144f), glm::vec3(0.393548f, 0.271906f, 0.166721f), 0.2f },   // bronze
	{ glm::vec3(0.25f, 0.25f, 0.25f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.774597f, 0.774597f, 0.774597f), 0.6f },   // chrome
	{ glm::vec3(0.19125f, 0.0735f, 0.0225f), glm::vec3(0.7038f, 0.27048f, 0.0828f), glm::vec3(0.256777f, 0.137622f, 0.086014f), 0.1f },   // copper
	{ glm::vec3(0.24725f, 0.1995f, 0.0745f), glm::vec3(0.75164f, 0.60648f, 0.22648f), glm::vec3(0.628281f, 0.555802f, 0.366065f), 0.4f },   // gold
	{ glm::vec3(0.19225f, 0.19225f, 0.19225f), glm::vec3(0.50754f, 0.50754f, 0.50754f), glm::vec3(0.508273f, 0.508273f, 0.508273f), 0.4f },   // silver
	{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.01f, 0.01f, 0.01f), glm::vec3(0.50f, 0.50f, 0.50f), 0.25f },   // black plastic
	{ glm::vec3(0.0f, 0.1f, 0.06f), glm::vec3(0.0f, 0.50980392f, 0.50980392f), glm::vec3(0.50196078f, 0.50196078f, 0.50196078f), 0.25f },   // cyan plastic
	{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.35f, 0.1f), glm::vec3(0.45f, 0.55f, 0.45f), 0.25f },   // green plastic
	{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.7f, 0.6f, 0.6f), 0.25f },   // red plastic
	{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.55f, 0.55f, 0.55f), glm::vec3(0.70f, 0.70f, 0.70f), 0.25f },   // white plastic
	{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.60f, 0.60f, 0.50f), 0.25f },   // yellow plastic
	{ glm::vec3(0.02f, 0.02f, 0.02f), glm::vec3(0.01f, 0.01f, 0.01f), glm::vec3(0.4f, 0.4f, 0.4f), 0.078125f },   // black rubber
	{ glm::vec3(0.0f, 0.05f, 0.05f), glm::vec3(0.4f, 0.5f, 0.5f), glm::vec3(0.04f, 0.7f, 0.7f), 0.078125f },   // cyan rubber
	{ glm::vec3(0.0f, 0.05f, 0.0f), glm::vec3(0.4f, 0.5f, 0.4f), glm::vec3(0.04f, 0.7f, 0.04f), 0.078125f },   // green rubber
	{ glm::vec3(0.05f, 0.0f, 0.0f), glm::vec3(0.5f, 0.4f, 0.4f), glm::vec3(0.7f, 0.04f, 0.04f), 0.078125f },   // red rubber
	{ glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f), 0.078125f },   // white rubber
	{ glm::vec3(0.05f, 0.05f, 0.0f), glm::vec3(0.5f, 0.5f, 0.4f), glm::vec3(0.7f, 0.7f, 0.04f), 0.078125f }    // yellow rubber
};

void failOnCondition(bool cond, const std::function<void()>& cleanup) {
	if (cond) {
		cleanup();
		exit(-1);
	}
}

GLFWwindow* createWindow() {

	auto& context = dlb::ApplicationSingleton::getInstance();
	auto& window_dims = context.getWindowDims();

	GLFWwindow* window = glfwCreateWindow(
		window_dims.x,
		window_dims.y,
		context.getWindowTitle().c_str(),
		NULL,
		NULL);

	return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	dlb::ApplicationSingleton::getInstance().setWindowDimsH(height);
	dlb::ApplicationSingleton::getInstance().setWindowDimsW(width);
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{

	auto& context = dlb::ApplicationSingleton::getInstance();

	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	float xoffset = xpos - context.getLastCursor().x;
	float yoffset = context.getLastCursor().y - ypos; // reversed since y-coordinates go from bottom to top
	context.setLastCursorX(xpos);
	context.setLastCursorY(ypos);

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	context.getCamera().updateDirection(glm::vec2(xoffset, yoffset));
}

void proccessInput(GLFWwindow* window) {

	auto& context = dlb::ApplicationSingleton::getInstance();

	auto& io = ImGui::GetIO();

	if (io.WantCaptureMouse) {
		// Enable the cursor when ImGui is capturing input
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	} else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}
		else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {

			std::cout << "[WIREFRAME MODE: ]" << context.getWireframeMode() << std::endl;

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
}

int main_() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


	auto window = createWindow();
	failOnCondition(window == NULL, [] {glfwTerminate(); });

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	auto& context = dlb::ApplicationSingleton::getInstance();

	glViewport(0, 0, context.getWindowDims().x, context.getWindowDims().y);
	glEnable(GL_DEPTH_TEST);

	// Initialize ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	auto& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

#pragma region Callbacks
	/*
	* Set the according viewport everytime the user resizes the window
	*/
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwMaximizeWindow(window);
	glfwSetCursorPosCallback(window, mouse_callback);
#pragma endregion

#pragma region Shader programs
	dlb::ShaderProgramBuilder spb{};

	spb
		.vertexShader("C:\\Users\\Diego\\Documents\\Code\\LearnOpenGL\\resources\\shaders\\multiple-light.vert")
		.fragmentShader("C:\\Users\\Diego\\Documents\\Code\\LearnOpenGL\\resources\\shaders\\multiple-light.frag");

	const auto block_shaders = spb.build();

	const auto light_source_shaders = dlb::ShaderProgramBuilder()
		.vertexShader("C:\\Users\\Diego\\Documents\\Code\\LearnOpenGL\\resources\\shaders\\light_source.vert")
		.fragmentShader("C:\\Users\\Diego\\Documents\\Code\\LearnOpenGL\\resources\\shaders\\light_source.frag")
		.build();
#pragma endregion

#pragma region Texture2DGroup Loading

	stbi_set_flip_vertically_on_load(true);

	auto light_source_texture = dlb::Texture2DGroupBuilder()
		.configure_new()
		.path("C:\\Users\\Diego\\Documents\\Code\\LearnOpenGL\\resources\\img\\redstone-lamp.png")
		.configure_new()
		.path("C:\\Users\\Diego\\Documents\\Code\\LearnOpenGL\\resources\\img\\redstone-lamp-specular.png")
		.build();

	auto container_texture = dlb::Texture2DGroupBuilder()
		.configure_new()
		.path("C:\\Users\\Diego\\Documents\\Code\\LearnOpenGL\\resources\\container2.png")
		.configure_new()
		.path("C:\\Users\\Diego\\Documents\\Code\\LearnOpenGL\\resources\\container2_specular.png")
		.build();
#pragma endregion

#pragma region Creating renderable objects

	std::vector<glm::vec3> positions {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	/*
	for (int i = 0; i < 16; i++)
		for (int j = 0; j < 16; j++)
			positions.push_back(glm::vec3(1.0f * i, 0.0f, 1.0f * j));
	*/
	
	dlb::Renderable light_source;
	glm::vec3 light_source_position{ 0.0f, 10.0f, 0.0f };
	glm::vec3 light_specular{ 1.0f };
	glm::vec3 light_diffuse{ 0.5f };
	glm::vec3 light_ambient{ 0.2f };
	light_source.setShaderProgram(&light_source_shaders);
	light_source.feedData(std::move(std::vector<float>(vertices)), 36);
	light_source.configureVertexAttributes(GL_ARRAY_BUFFER, GL_STATIC_DRAW,
		[] {
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(vertices[0]), NULL);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(vertices[0]), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(1);
		});
	light_source.setTextures(&light_source_texture);

	dlb::Renderable cube;
	cube.setShaderProgram(&block_shaders);
	cube.feedData(std::move(std::vector<float>(vertices)), 36);
	cube.configureVertexAttributes(GL_ARRAY_BUFFER, GL_STATIC_DRAW,
		[] {
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(vertices[0]), NULL);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(vertices[0]), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(vertices[0]), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(2);
		});
	cube.setMaterial(materials[0]);
	cube.setUseMaterial(false);
	cube.setTextures(&container_texture);
#pragma endregion

#pragma region Scene Configuration
	float specular_strength = 0.5;
	float ambient_strength = 0.5;
	float shininess = 16.0f;

	glm::vec3 bg_color{ 0.0f };

	float light_point_constant = 1.0f;
	float light_point_linear = 0.7f;
	float light_point_quad = 1.8f;

#pragma region Light Bulbs
	std::vector<glm::vec3> light_bulbs_positions {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	glm::vec3 light_bulbs_colors[] = {
		glm::vec3(1.0f, 0.6f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0, 0.0),
		glm::vec3(0.2f, 0.2f, 1.0f)
	};
#pragma endregion

#pragma endregion

	while (!glfwWindowShouldClose(window)) {
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)context.getWindowDims().x / (float)context.getWindowDims().y, 0.1f, 100.0f);

		proccessInput(window);
		context.updateTime();

		glClearColor(bg_color.r, bg_color.g, bg_color.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#pragma region ImGui Panel
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(300, 500));
		ImGui::Begin("Scene Configuration");


		ImGui::Text(std::format("FPS: {}/{}",
			context.getFrames(), 1000.0f / (float)context.getFrames()).c_str());

		ImGui::Text(std::format("X: {} Y: {} Z: {}",
			context.getCamera().getPosition().x, context.getCamera().getPosition().y, context.getCamera().getPosition().z).c_str());

		ImGui::ColorEdit3("Bg Color", glm::value_ptr(bg_color)); // Adjust light source color

		ImGui::Text("Light Point Configuration");
		ImGui::ColorEdit3("Ambient", glm::value_ptr(light_ambient)); // Adjust light source color
		ImGui::ColorEdit3("Diffuse", glm::value_ptr(light_diffuse)); // Adjust light source color
		ImGui::ColorEdit3("Specular", glm::value_ptr(light_specular)); // Adjust light source color

		ImGui::SliderFloat("Constant", &light_point_constant, -5.0f, 5.0f);
		ImGui::SliderFloat("Linear", &light_point_linear, -2.0f, 2.0f);
		ImGui::SliderFloat("Quad", &light_point_quad, -1.0f, 2.0f);

		ImGui::Text("Light Source Configuration");
		ImGui::SliderFloat("X Position", &light_source_position.x, -20.0f, 20.0f);
		ImGui::SliderFloat("Y Position", &light_source_position.y, -20.0f, 20.0f);
		ImGui::SliderFloat("Z Position", &light_source_position.z, -20.0f, 20.0f);
		ImGui::SliderFloat("Shininess", &shininess, 4.0f, 1024.0f);
		ImGui::End();
#pragma endregion

		/*
		light_source.render(
			[&](const dlb::ShaderProgram* sp, dlb::Texture2DGroup* texs) {
				sp->use();
				texs->use();

				//light_source_position.x = cos(glfwGetTime()) * 20.0F;
				//light_source_position.z = sin(glfwGetTime()) * 20.0F;

				sp->setUniform("u_light_color", glm::vec3(1.0f));
				const glm::mat4 model = glm::scale(glm::translate(glm::mat4(1.0F), light_source_position), glm::vec3(0.1f));
				const glm::mat4 view = context.getCamera().getView();

				sp->setUniform("u_model", model);
				sp->setUniform("u_view", view);
				sp->setUniform("u_projection", projection);
			});
		*/

		const glm::vec3 light_point_position = glm::vec3(6.0f, 5.0f, -5.0f);

		for (int i = 0; i < light_bulbs_positions.size(); i++) {
			// render a light point
			light_source.render(
				[&](const dlb::ShaderProgram* sp, dlb::Texture2DGroup* texs) {
					sp->use();
					texs->use();

					//light_source_position.x = cos(glfwGetTime()) * 20.0F;
					//light_source_position.z = sin(glfwGetTime()) * 20.0F;

					sp->setUniform("u_light_color", glm::vec3(1.0f));
					const glm::mat4 model = glm::scale(
						glm::translate(glm::mat4(1.0F), light_bulbs_positions[i]),
						glm::vec3(0.1f));
					const glm::mat4 view = context.getCamera().getView();

					sp->setUniform("u_model", model);
					sp->setUniform("u_view", view);
					sp->setUniform("u_projection", projection);
				});
		}
		// render a light point
		light_source.render(
			[&](const dlb::ShaderProgram* sp, dlb::Texture2DGroup* texs) {
				sp->use();
				texs->use();

				//light_source_position.x = cos(glfwGetTime()) * 20.0F;
				//light_source_position.z = sin(glfwGetTime()) * 20.0F;

				sp->setUniform("u_light_color", glm::vec3(1.0f));
				const glm::mat4 model = glm::scale(
						glm::translate(glm::mat4(1.0F), light_point_position),
						glm::vec3(0.1f));
				const glm::mat4 view = context.getCamera().getView();

				sp->setUniform("u_model", model);
				sp->setUniform("u_view", view);
				sp->setUniform("u_projection", projection);
			});

		for (int i = 0; i < positions.size(); i++) {
			cube.render(
				[&](const dlb::ShaderProgram* sp, dlb::Texture2DGroup* texs) {
					glm::mat4 model = glm::translate(glm::mat4(1.0F), positions[i]);
					model = glm::rotate(model, glm::radians(i * 20.0f), glm::vec3(0.1f, 0.6f, -0.7f));
					const glm::mat4 view = context.getCamera().getView();

					sp->use();
					texs->use();

					sp->setUniform("u_light.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
					sp->setUniform("u_light.ambient", glm::vec3(0.3f, 0.24f, 0.14f));
					sp->setUniform("u_light.diffuse", glm::vec3(0.7f, 0.42f, 0.26f));
					sp->setUniform("u_light.specular", glm::vec3(0.5f, 0.5f, 0.5f));

					sp->setUniform("u_light_points[0].position", light_bulbs_positions[0]);
					sp->setUniform("u_light_points[0].ambient", light_bulbs_colors[0]);
					sp->setUniform("u_light_points[0].diffuse", light_bulbs_colors[0] * 0.1F);
					sp->setUniform("u_light_points[0].specular", light_bulbs_colors[0]);
					sp->setUniform("u_light_points[0].constant", 1.0f);
					sp->setUniform("u_light_points[0].linear", 0.09f);
					sp->setUniform("u_light_points[0].quadratic", 0.032f);
					// point light 2
					sp->setUniform("u_light_points[1].position", light_bulbs_positions[1]);
					sp->setUniform("u_light_points[1].ambient", light_bulbs_colors[1]);
					sp->setUniform("u_light_points[1].diffuse", light_bulbs_colors[1] * 0.1F);
					sp->setUniform("u_light_points[1].specular", light_bulbs_colors[1]);
					sp->setUniform("u_light_points[1].constant", 1.0f);
					sp->setUniform("u_light_points[1].linear", 0.09f);
					sp->setUniform("u_light_points[1].quadratic", 0.032f);
					// point light 3
					sp->setUniform("u_light_points[2].position", light_bulbs_positions[2]);
					sp->setUniform("u_light_points[2].ambient", light_bulbs_colors[2]);
					sp->setUniform("u_light_points[2].diffuse", light_bulbs_colors[2] * 0.1F);
					sp->setUniform("u_light_points[2].specular", light_bulbs_colors[2]);
					sp->setUniform("u_light_points[2].constant", 1.0f);
					sp->setUniform("u_light_points[2].linear", 0.09f);
					sp->setUniform("u_light_points[2].quadratic", 0.032f);
					// point light 4
					sp->setUniform("u_light_points[3].position", light_bulbs_positions[3]);
					sp->setUniform("u_light_points[3].ambient", light_bulbs_colors[3]);
					sp->setUniform("u_light_points[3].diffuse", light_bulbs_colors[3] * 0.1F);
					sp->setUniform("u_light_points[3].specular", light_bulbs_colors[3]);
					sp->setUniform("u_light_points[3].constant", 1.0f);
					sp->setUniform("u_light_points[3].linear", 0.09f);
					sp->setUniform("u_light_points[3].quadratic", 0.032f);

					sp->setUniform("u_material.diffuse", 0);
					sp->setUniform("u_material.specular", 1);
					//sp->setUniform("u_material.emission", 2);
					sp->setUniform("u_material.shininess", shininess);

					sp->setUniform("u_spotlight.position", context.getCamera().getPosition());
					sp->setUniform("u_spotlight.direction", context.getCamera().getDirection());
					sp->setUniform("u_spotlight.cutoff", glm::cos(glm::radians(12.5f)));

					sp->setUniform("u_eye_position", context.getCamera().getPosition());

					sp->setUniform("u_model", model);
					sp->setUniform("u_view", view);
					sp->setUniform("u_projection", projection);
				});
		}

		// Render ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

