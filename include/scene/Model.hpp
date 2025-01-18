#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "Mesh.hpp"
#include "Texture.hpp"

namespace scene {

	struct AABB {
		glm::vec3 min;
		glm::vec3 max;

		AABB translate(const glm::mat4& t) {
			AABB result = { min, max };
			glm::vec4 m = t * glm::vec4(min, 1.0f);
			result.min.x = m.x;
			result.min.y = m.y;
			result.min.z = m.z;

			m = t * glm::vec4(max, 1.0f);
			result.max.x = m.x;
			result.max.y = m.y;
			result.max.z = m.z;

			return result;
		}

		bool test(const AABB& b) {
			return (
				min.x <= b.max.x &&
				max.x >= b.min.x &&
				min.y <= b.max.y &&
				max.y >= b.min.y &&
				min.z <= b.max.z &&
				max.z >= b.min.z
			);
		}
	};

	class Model {
	public:
		Model(const char* path, uint flags = ModelFlags::UseTextures)
			:aabb_mesh_{} {
			error = false;
			model_ = glm::mat4(1.0f);
			flags_ = flags;
			load_model(path);
		}

	public:
		void draw(const dlb::ShaderProgram& sp, const glm::mat4& transformation, const glm::vec3& bb_color);

		void translate(const glm::vec3& position) {
			model_ = glm::translate(model_, position);
		}

		void scale(float x) {
			model_ = glm::scale(model_, glm::vec3(x));
		}

		AABB& getAABB() {
			return aabb_;
		}

		/*
		* Checks if two models are colliding
		*/
		bool AABBTest(Model& other, const glm::vec3& this_position, const glm::vec3& other_position);

	private:
		void load_model(const char* path);
		void process_node(aiNode* node, const aiScene* scene);
		Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
		Material process_material(aiMaterial* material, const aiScene* scene);
		void load_material_textures(dlb::Texture2DGroupBuilder& builder, aiMaterial* mat, aiTextureType type);

		void createAABB();
		void createAABBMesh();

	private:
		std::vector<Mesh> meshes_;
		BasicMesh aabb_mesh_;
		std::string directory_;
		glm::mat4 model_;

		AABB aabb_;

		bool error;
		// by default all models should use textures instead of materials.
		uint flags_;
	};
}