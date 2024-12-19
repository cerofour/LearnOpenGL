#include <glad/glad.h>

#include <gl/GL.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgram.hpp"

#include "io/FileReader.hpp"

namespace dlb {
	ShaderProgram ShaderProgramBuilder::build() {
		this->shader_program = glCreateProgram();

		for (auto& shader : this->shaders) {
			shader.shaderId = glCreateShader(shader.type);
			std::string shaderContent = FileReader::read(shader.path);
			const char* data = shaderContent.data();
			glShaderSource(shader.shaderId, 1, &data, NULL);
			glCompileShader(shader.shaderId);
			checkCompileErrors(shader.shaderId);
			glAttachShader(this->shader_program, shader.shaderId);
		}

		glLinkProgram(shader_program);

		checkCompileErrors(-1);

		for (auto& shader : this->shaders)
			glDeleteShader(shader.shaderId);

		return ShaderProgram{ shader_program };
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
			glGetProgramiv(this->shader_program, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(this->shader_program, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			}
		}
	}

	void ShaderProgram::setUniform(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(program_id, name.c_str()), static_cast<int>(value));
	}

	void ShaderProgram::setUniform(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
	}

	void ShaderProgram::setUniform(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
	}

	void ShaderProgram::setUniform(const std::string& name, const glm::vec2& value) const {
		glUniform2fv(glGetUniformLocation(program_id, name.c_str()), 1, glm::value_ptr(value));
	}

	void ShaderProgram::setUniform(const std::string& name, const glm::vec3& value) const {
		glUniform3fv(glGetUniformLocation(program_id, name.c_str()), 1, glm::value_ptr(value));
	}

	void ShaderProgram::setUniform(const std::string& name, const glm::vec4& value) const {
		glUniform4fv(glGetUniformLocation(program_id, name.c_str()), 1, glm::value_ptr(value));
	}

	void ShaderProgram::setUniform(const std::string& name, const glm::mat2& value) const {
		glUniformMatrix2fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void ShaderProgram::setUniform(const std::string& name, const glm::mat3& value) const {
		glUniformMatrix3fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void ShaderProgram::setUniform(const std::string& name, const glm::mat4& value) const {
		glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void ShaderProgram::use() const {
		glUseProgram(program_id);
	}

};