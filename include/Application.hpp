#pragma once

#include <string>
#include <format>
#include <iostream>

#include "Texture.hpp"
#include "Camera.hpp"
#include "ShaderProgram.hpp"
#include "scene/Model.hpp"

struct GLFWwindow;

namespace dlb {

	struct GlobalLight {
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		glm::vec3 direction;
	};

	class ApplicationSingleton {

	private:
		ApplicationSingleton()
			:camera{glm::vec3(0.0f, 0.0f, -5.0f)},
			bg_color{ 0.0f, 0.0f, 0.0f }
		{
			window_title = "physics engine";
			window_dims.x = 1000;
			window_dims.y = 700;

			init();

			pause_ecs_ = false;
			
			last_cursor_pos = { window_dims.x / 2.0f, window_dims.y / 2.0f };

			delta_time = 0.0f;
			last_frame = 0.0f;

			wireframe_mode = false;

			global_light = {
				.ambient = glm::vec3(0.3f, 0.24f, 0.14f),
				.diffuse = glm::vec3(0.7f, 0.42f, 0.26f),
				.specular = glm::vec3(0.5f, 0.5f, 0.5f),
				.direction = glm::vec3(-0.2f, -1.0f, -0.3f),
			};

			texture_pool = &dlb::Texture2DPool::getInstance();

			// load Bounding box shader
			aabb_shader_ = addShader(dlb::ShaderProgramBuilder{}
				.vertexShader("C:\\Users\\Diego\\Documents\\Code\\LearnOpenGL\\resources\\shaders\\AABB.vert")
				.fragmentShader("C:\\Users\\Diego\\Documents\\Code\\LearnOpenGL\\resources\\shaders\\AABB.frag"));
		}
	private:
		void init();
		//void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
		//GLFWwindow* createWindow(int w, int h, const char* title);
		//void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	public:
		static ApplicationSingleton& getInstance() {
			static ApplicationSingleton instance{};
			return instance;
		}

		inline const glm::vec2& getWindowDims() const {
			return window_dims;
		}

		inline Texture2DPool* getTexture2DPool() {
			return texture_pool;
		}

		inline const std::string& getWindowTitle() const {
			return window_title;
		}

		inline double getDeltaTime() const {
			return delta_time;
		}

		inline double getLastFrame() const {
			return last_frame;
		}

		inline void setWindowDimsW(float value) {
			window_dims.x = value;
		}

		inline void setWindowDimsH(float value) {
			window_dims.y = value;
		}

		inline bool& getPauseEcs() {
			return pause_ecs_;
		}

		inline dlb::Camera& getCamera() {
			return camera;
		}

		inline bool getWireframeMode() {
			return wireframe_mode;
		}

		inline void setWireframeMode(bool val) {
			wireframe_mode = val;
		}

		inline glm::vec2& getLastCursor() {
			return last_cursor_pos;
		}

		void setLastCursorX(float val) {
			last_cursor_pos.x = val;
		}

		void setLastCursorY(float val) {
			last_cursor_pos.y = val;
		}

		int getFrames() {
			return frames;
		}

		auto& getGlobalLight() {
			return global_light;
		}

		auto& getBgColor() {
			return bg_color;
		}

		void error(const std::string& error_msg, const char* filename = "NOFILE", const char* function = "NOFUNC") const {
			std::cerr << std::format("[ERROR]: {}:{} {}\n", filename, function, error_msg);
		}

		uint addModel(const char* path, uint flags) {
			models_.push_back({ path, flags });
			return models_.size() - 1;
		}

		uint addShader(dlb::ShaderProgramBuilder& builder) {
			shaders_.push_back(builder.build());
			return shaders_.size() - 1;
		}

		scene::Model& getModel(uint model_id) {
			if (model_id >= models_.size())
				abort();

			return models_[model_id];
		}

		const dlb::ShaderProgram& getShader(uint id) const {
			return shaders_[id];
		}

		uint getAABBShader() const {
			return aabb_shader_;
		}

		void updateTime();

		GLFWwindow* getWindow() {
			return window_;
		}

	private:
		GLFWwindow* window_;
		glm::vec2 window_dims;
		glm::vec2 last_cursor_pos;
		glm::vec3 bg_color;
		std::string window_title;

		double delta_time;
		double last_frame;
		double last_time = 0.0F;

		dlb::Camera camera;

		bool wireframe_mode;
		bool pause_ecs_;
		//bool init_;

		GlobalLight global_light;

		int frames = 0;

		dlb::Texture2DPool* texture_pool;

		std::vector<scene::Model> models_;
		std::vector<dlb::ShaderProgram> shaders_;
		uint aabb_shader_;
	};
}