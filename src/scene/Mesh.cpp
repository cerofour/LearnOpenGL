#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <format>
#include <limits>

#include "scene/Mesh.hpp"
#include "Application.hpp"

namespace scene {
	/*
	* Configures the VAO, VBO and EBO of this mesh
	* The first vec3 of a vertex is the position vertices,
	*	the second vec3 is the normals vectors
	*	and the last vec2 is the texture coordinates.
	*/
	void Mesh::defaultSetup() {
		auto& context = dlb::ApplicationSingleton::getInstance();
		glGenVertexArrays(1, &VAO);

		auto error = glGetError();

		if (error != GL_NO_ERROR) {
			context.error(std::format("Error rendering mesh, glGetError returned {}.", error), __FILE__, __FUNCTION__);
		}

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		/*
		* Feed data to the GPU
		* - Position attribute (3 floats)
		* - Normal attribute (3 floats)
		* - Texture coordinate (2 floats)
		*/
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(uint), &indices_[0], GL_STATIC_DRAW);

		/*
		* Set the vertex attribute pointers
		* vertex positions
		*/
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));

		glBindVertexArray(0);
	}

	static void setLightingUniforms(const dlb::ShaderProgram& sp) {
		auto& context = dlb::ApplicationSingleton::getInstance();
		const auto& light = context.getGlobalLight();

		sp.setUniform("u_light.direction", light.direction);
		sp.setUniform("u_light.ambient", light.ambient);
		sp.setUniform("u_light.diffuse", light.diffuse);
		sp.setUniform("u_light.specular", light.specular);
		sp.setUniform("u_eye_position", context.getCamera().getPosition());
	}

	void Mesh::draw(const dlb::ShaderProgram& sp, const glm::mat4& transformation, uint flags) {

		uint n_diffuse = 0;
		uint n_specular = 0;

		auto& context = dlb::ApplicationSingleton::getInstance();

		const auto& texs = textures_.getTextures();

		sp.use();

		if (flags & UseTextures) {
			for (int i = 0; i < texs.size(); i++) {
				glActiveTexture(GL_TEXTURE0 + i);

				if (texs[i].type == dlb::Texture2DType::Diffuse)
					sp.setUniform(std::format("u_material.texture_diffuse{}", n_diffuse++), i);

				else if (texs[i].type == dlb::Texture2DType::Specular)
					sp.setUniform(std::format("u_material.texture_specular{}", n_specular++), i);

				glBindTexture(GL_TEXTURE_2D, texs[i].id);
			}

			// TODO: FIX THIS
			sp.setUniform("u_material.shininess", 64.0f);
		}
		else if (flags & UseMaterials) {
			sp.setUniform("u_material.diffuse", material_.diffuse);
			sp.setUniform("u_material.ambient", material_.ambient);
			sp.setUniform("u_material.specular", material_.specular);
			sp.setUniform("u_material.shininess", material_.shininess);
		}
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)context.getWindowDims().x / (float)context.getWindowDims().y, 0.1f, 100.0f);

		sp.setUniform("u_model", transformation);
		sp.setUniform("u_view", context.getCamera().getView());
		sp.setUniform("u_projection", projection);

		setLightingUniforms(sp);

		glBindVertexArray(VAO);

		auto error = glGetError();

		if (error != GL_NO_ERROR) {
			context.error(std::format("Error rendering mesh, glGetError returned {}.", error), __FILE__, __FUNCTION__);
		}

		glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);

		error = glGetError();

		if (error != GL_NO_ERROR) {
			context.error(std::format("Error rendering mesh, glGetError returned {}.", error), __FILE__, __FUNCTION__);
		}

		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}
	glm::vec3 Mesh::getMinCoords() {
		glm::vec3 min{std::numeric_limits<float>::infinity()};

		for (int i = 0; i < vertices_.size(); i++) {

			auto x = vertices_[i].position.x;
			auto y = vertices_[i].position.y;
			auto z = vertices_[i].position.z;

			if (x < min.x)
				min.x = x;
			if (y < min.y)
				min.y = y;
			if (z < min.z)
				min.z = z;
		}

		return min;
	}
	glm::vec3 Mesh::getMaxCoords() {
		glm::vec3 max{ -std::numeric_limits<float>::infinity() };

		for (int i = 0; i < vertices_.size(); i++) {

			auto x = vertices_[i].position.x;
			auto y = vertices_[i].position.y;
			auto z = vertices_[i].position.z;

			if (x > max.x)
				max.x = x;
			if (y > max.y)
				max.y = y;
			if (z > max.z)
				max.z = z;
		}

		return max;
	}

	void BasicMesh::feed(std::vector<BasicVertex>&& vertices, std::vector<uint> indices) {
		vertices_ = std::move(vertices);
		indices_ = std::move(indices);

		glGenVertexArrays(1, &VAO);

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		/*
		* Feed data to the GPU
		* - Position attribute (3 floats)
		*/
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(BasicVertex), &vertices_[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(uint), &indices_[0], GL_STATIC_DRAW);

		/*
		* Set the vertex attribute pointers
		* vertex positions
		*/
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BasicVertex), (void*)0);
	}

	void BasicMesh::draw(const dlb::ShaderProgram& sp, const glm::mat4& transformation, const glm::vec3& color) {
		auto& context = dlb::ApplicationSingleton::getInstance();

		sp.use();

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)context.getWindowDims().x / (float)context.getWindowDims().y, 0.1f, 100.0f);

		sp.setUniform("u_color", color);
		sp.setUniform("u_model", transformation);
		sp.setUniform("u_view", context.getCamera().getView());
		sp.setUniform("u_projection", projection);

		glBindVertexArray(VAO);

		auto error = glGetError();

		if (error != GL_NO_ERROR) {
			context.error(std::format("Error rendering mesh, glGetError returned {}.", error), __FILE__, __FUNCTION__);
		}

		glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
		error = glGetError();

		if (error != GL_NO_ERROR) {
			context.error(std::format("Error rendering mesh, glGetError returned {}.", error), __FILE__, __FUNCTION__);
		}

		glBindVertexArray(0);
	}
}