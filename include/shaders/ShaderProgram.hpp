#pragma once


#include <string>
#include <vector>

#include <gl/GL.h>

#include <glm/glm.hpp>

namespace dlb {

	class ShaderProgram {
	public:
		ShaderProgram(int id)
			:program_id(id) {}

		~ShaderProgram() {
			glDeleteProgram(program_id);
		}

		int getProgramId() {
			return program_id;
		}

		void setUniform(const std::string& name, bool value) const;
		void setUniform(const std::string& name, int value) const;
		void setUniform(const std::string& name, float value) const;
		void setUniform(const std::string& name, const glm::vec2& value) const;
		void setUniform(const std::string& name, const glm::vec3& value) const;
		void setUniform(const std::string& name, const glm::vec4& value) const;
		void setUniform(const std::string& name, const glm::mat2& value) const;
		void setUniform(const std::string& name, const glm::mat3& value) const;
		void setUniform(const std::string& name, const glm::mat4& value) const;

		void use() const;

	private:
		int program_id = -1;
	};

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

		ShaderProgram build();
		void checkCompileErrors(int current_shader);

	private:
		std::vector<Shader> shaders;
		int shader_program = -1;
	};
};