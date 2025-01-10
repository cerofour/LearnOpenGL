#pragma once

#include <string>
#include <format>
#include <iostream>

#include "Texture.hpp"
#include "Camera.hpp"

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
			:camera{glm::vec3(0.0f, 0.0f, -5.0f)}
		{
			window_dims.x = 800;
			window_dims.y = 600;

			last_cursor_pos = { window_dims.x / 2.0f, window_dims.y / 2.0f };

			window_title = "Learn OpenGL";
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
		}

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

		const auto& getGlobalLight() const {
			return global_light;
		}

		void error(const std::string& error_msg, const char* filename = "NOFILE", const char* function = "NOFUNC") const {
			std::cerr << std::format("[ERROR]: {}:{} {}\n", filename, function, error_msg);
		}

		void updateTime();

	private:
		glm::vec2 window_dims;
		glm::vec2 last_cursor_pos;
		std::string window_title;

		double delta_time;
		double last_frame;
		double last_time = 0.0F;

		dlb::Camera camera;

		bool wireframe_mode;

		GlobalLight global_light;

		int frames = 0;

		dlb::Texture2DPool* texture_pool;
	};
}