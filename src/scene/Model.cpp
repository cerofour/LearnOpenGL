#include <iostream>
#include <format>

#include "scene/Model.hpp"
#include "Application.hpp"

namespace scene {
	void Model::draw(const dlb::ShaderProgram& sp, const glm::mat4& transformation, const glm::vec3& bb_color = {0.0f, 0.0f, 0.0f}) {
		const auto& context = dlb::ApplicationSingleton::getInstance();

		for (auto& mesh : meshes_) {
			mesh.draw(sp, transformation, flags_);
		}

		if ((flags_ & DrawAABB))
			aabb_mesh_.draw(context.getShader(context.getAABBShader()), transformation, bb_color);
	}

	bool Model::AABBTest(Model& other, const glm::vec3& this_position, const glm::vec3& other_position) {
		glm::mat4 this_transform = glm::translate(glm::mat4(1.0f), this_position);
		glm::mat4 other_transform = glm::translate(glm::mat4(1.0f), other_position);

		AABB thisAABB = aabb_.translate(this_transform);
		AABB otherAABB = other.aabb_.translate(other_transform);

		return thisAABB.test(otherAABB);
	}

	void Model::load_model(const char* path) {
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cerr << "Error: load_model: " << importer.GetErrorString() << std::endl;
			error = true;
			return;
		}

		const std::string p{ path };

		directory_ = p.substr(0, p.find_last_of('\\') + 1);
		process_node(scene->mRootNode, scene);

		createAABB();

		if (flags_ & DrawAABB)
			createAABBMesh();
	}

	void Model::process_node(aiNode* node, const aiScene* scene) { 
		for (int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes_.push_back(std::move(process_mesh(mesh, scene)));
		}

		for (int i = 0; i < node->mNumChildren; i++)
			process_node(node->mChildren[i], scene);
	}

	Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene) {
		std::vector<Vertex> vertices{};
		std::vector<uint> indices{};
		std::vector<dlb::Texture2D> textures{};

		for (int i = 0; i < mesh->mNumVertices; i++) {
			Vertex vert;
			vert.position.x = mesh->mVertices[i].x;
			vert.position.y = mesh->mVertices[i].y;
			vert.position.z = mesh->mVertices[i].z;

			vert.normal.x = mesh->mNormals[i].x;
			vert.normal.y = mesh->mNormals[i].y;
			vert.normal.z = mesh->mNormals[i].z;


			if (flags_ & ModelFlags::UseTextures) {
				if (mesh->mTextureCoords[0]) {
					vert.tex_coords.x = mesh->mTextureCoords[0][i].x;
					vert.tex_coords.y = mesh->mTextureCoords[0][i].y;
				}
				else {
					vert.tex_coords = { 0.0f, 0.0f };
				}
			}

			vertices.push_back(vert);
		}

		for (int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		dlb::Texture2DGroupBuilder tex_group_builder{};

		Material mat{};

		if (mesh->mMaterialIndex >= 0) {
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			if (flags_ & UseMaterials)
				mat = process_material(material, scene);

			else if (flags_ & UseTextures) {
				load_material_textures(tex_group_builder, material, aiTextureType_DIFFUSE);
				load_material_textures(tex_group_builder, material, aiTextureType_SPECULAR);
			}
		}

		return Mesh(std::move(vertices), std::move(indices), tex_group_builder.build(), mat);
	}

	Material Model::process_material(aiMaterial* material, const aiScene* scene) {
		Material mat{};

		aiColor3D color{ 0.0f };

		material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		mat.diffuse = { color.r, color.g, color.b };
		
		material->Get(AI_MATKEY_COLOR_AMBIENT, color);
		mat.ambient = { color.r, color.g, color.b };

		material->Get(AI_MATKEY_COLOR_SPECULAR, color);
		mat.specular = { color.r, color.g, color.b };

		float shininess{ 0.0f };

		material->Get(AI_MATKEY_SHININESS, shininess);
		mat.shininess = shininess;

		return mat;
	}

	void Model::load_material_textures(dlb::Texture2DGroupBuilder& builder, aiMaterial* material, aiTextureType type) {
		for (int i = 0; i < material->GetTextureCount(type); i++) {
			aiString path;
			material->GetTexture(type, i, &path);
			builder.configure_new()
				.path(std::format("{}{}", directory_, path.C_Str()))
				.type((type == aiTextureType_DIFFUSE) ? dlb::Texture2DType::Diffuse : dlb::Texture2DType::Specular);
		}
	}

	void Model::createAABB() {
		glm::vec3 min{ std::numeric_limits<float>::infinity() };
		glm::vec3 max{ -std::numeric_limits<float>::infinity()};

		for (int i = 0; i < meshes_.size(); i++) {
			auto current_min = meshes_[i].getMinCoords();
			auto current_max = meshes_[i].getMaxCoords();

			if (min.x > current_min.x)
				min.x = current_min.x;

			if (min.y > current_min.y)
				min.y = current_min.y;

			if (min.z > current_min.z)
				min.z = current_min.z;

			if (max.x < current_max.x)
				max.x = current_max.x;

			if (max.y < current_max.y)
				max.y = current_max.y;

			if (max.z < current_max.z)
				max.z = current_max.z;
		}

		aabb_.min = min;
		aabb_.max = max;
	}
	void Model::createAABBMesh() {
		auto& m = aabb_.max;
		auto& i = aabb_.min;

		std::vector<BasicVertex> verts = {
			{glm::vec3(m.x, m.y, m.z)}, // m
			{glm::vec3(i.x, i.y, i.z)}, // i
			{glm::vec3(m.x, i.y, m.z)}, // a
			{glm::vec3(i.x, i.y, m.z)}, // b
			{glm::vec3(i.x, m.y, m.z)}, // c
			{glm::vec3(i.x, m.y, i.z)}, // d
			{glm::vec3(m.x, m.y, i.z)}, // e
			{glm::vec3(m.x, i.y, i.z)}, // f
		};

		std::vector<uint> indices = {
			0,2,3, //m a b
			0,4,3, //m c b
			0,2,7, 
			0,6,7,
			0,4,5,
			0,5,6,
			1,3,4,
			1,4,5,
			1,7,6,
			1,6,5,
			1,3,2,
			1,7,2,
		};

		aabb_mesh_.feed(std::move(verts), std::move(indices));
	}
}