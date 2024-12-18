#pragma once

#include <string>
#include <vector>

#include <gl/GL.h>

#include "io/FileReader.hpp"

namespace dlb {
	class ShaderProgramBuilder {
	public:

		struct Shader {

			/*
			* Path to the shader file, will be read and compiled
			*/
			std::string path;
			GLenum type;
			GLuint shaderId;
		};

		ShaderProgramBuilder()
			:shaders{}
		{}

		ShaderProgramBuilder& fragmentShader(const std::string& path) {
			shaders.push_back({ path, GL_FRAGMENT_SHADER });
			return *this;
		}

		ShaderProgramBuilder& vertexShader(const std::string& path) {
			shaders.push_back({ path, GL_VERTEX_SHADER });
			return *this;
		}

		int build() {
			this->shaderProgram = glCreateProgram();

			for (auto& shader : this->shaders) {
				shader.shaderId = glCreateShader(shader.type);
				std::string shaderContent = FileReader::read(shader.path);
				const char* data = shaderContent.data();
				glShaderSource(shader.shaderId, 1, &data, NULL);
				glCompileShader(shader.shaderId);
				glAttachShader(this->shaderProgram, shader.shaderId);
			}

			glLinkProgram(shaderProgram);

			for (auto& shader : this->shaders)
				glDeleteShader(shader.shaderId);

			return this->shaderProgram;
		}

	private:
		std::vector<Shader> shaders;
		int shaderProgram = -1;
	};
};