#include <glad/glad.h>

#include <gl/GL.h>

#include <iostream>

#include "ShaderProgram.hpp"

#include "io/FileReader.hpp"

namespace dlb {
	int ShaderProgramBuilder::build() {
		this->shaderProgram = glCreateProgram();

		for (auto& shader : this->shaders) {
			shader.shaderId = glCreateShader(shader.type);
			std::string shaderContent = FileReader::read(shader.path);
			const char* data = shaderContent.data();
			glShaderSource(shader.shaderId, 1, &data, NULL);
			glCompileShader(shader.shaderId);
			checkCompileErrors(shader.shaderId);
			glAttachShader(this->shaderProgram, shader.shaderId);
		}

		glLinkProgram(shaderProgram);

		checkCompileErrors(-1);

		for (auto& shader : this->shaders)
			glDeleteShader(shader.shaderId);

		return this->shaderProgram;
	}

	void ShaderProgramBuilder::checkCompileErrors(int current) {
		int success = 1;
		char infoLog[1024];

		if (current >= 0) {
			glGetShaderiv(current, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(current, 1024, NULL, infoLog);
				std::cout << "[SHADER BUILDER]" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		} else {
			glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(this->shaderProgram, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			}
		}
	}

};