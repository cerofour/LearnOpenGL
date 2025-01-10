#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

#include "Mesh.hpp"
#include "Texture.hpp"

namespace scene {
	class Model {
	public:
		Model(const char* path) {
			error = false;
			load_model(path);
		}

	public:
		void draw(const dlb::ShaderProgram& sp) {
			for (auto& mesh : meshes_) {
				mesh.draw(sp);
			}
		}

	private:
		void load_model(const char* path);
		void process_node(aiNode* node, const aiScene* scene);
		Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
		void load_material_textures(dlb::Texture2DGroupBuilder& builder, aiMaterial* mat, aiTextureType type);

	private:
		std::vector<Mesh> meshes_;
		std::string directory_;
		bool error;
	};
}