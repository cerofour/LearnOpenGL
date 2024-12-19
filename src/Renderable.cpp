#include <glad/glad.h>
#include <gl/gl.h>

#include "ShaderProgram.hpp"

#include "Renderable.hpp"

namespace dlb {
	Renderable::Renderable() {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);

		shader_program = nullptr;
	}

	Renderable::~Renderable() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}

	void Renderable::configureVertexAttributes(GLenum buffer_type, GLenum usage, const std::function<void(void)>& configure) {
		glBindVertexArray(vao);
		glBindBuffer(buffer_type, vbo);

		glBufferData(
			buffer_type,
			sizeof(vertices[0]) * vertices.size(),
			vertices.data(),
			usage);

		configure();
	}

	void Renderable::render(const std::function<void(const dlb::ShaderProgram*)>& pre_render) {
		if (do_render) {
			assert(shader_program != nullptr && "Shader Program is not defined");

			shader_program->use();
			glBindVertexArray(vao);

			pre_render(shader_program);

			glDrawArrays(GL_TRIANGLES, 0, vertices_no);
		}
	}
}