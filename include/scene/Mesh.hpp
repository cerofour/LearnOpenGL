#include <type_traits>
#include <vector>
#include <memory>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Types.hpp"
#include "Texture.hpp"
#include "ShaderProgram.hpp"

namespace scene {

	struct Vertex {

		static_assert(std::is_pod<glm::vec3>::value, "Vec3 type must be POD.");

		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 tex_coords;
	};

	static_assert(std::is_pod<Vertex>::value, "Vertex type must be POD.");

	class Mesh {
	public:
		Mesh(std::vector<Vertex>&& vertex, std::vector<uint>&& indices, dlb::Texture2DGroup&& texs)
		:vertices_(std::move(vertex)),
		indices_(std::move(indices)),
		textures_(std::move(texs))
		{
			VAO = 0;
			VBO = 0;
			EBO = 0;
			defaultSetup();
		}

		Mesh(Mesh&& x) noexcept
			:vertices_(std::move(x.vertices_)),
			indices_(std::move(x.indices_)),
			textures_(std::move(x.textures_)) {

			VAO = x.VAO;
			VBO = x.VBO;
			EBO = x.EBO;

			x.VAO = 0;
			x.VBO = 0;
			x.EBO = 0;
		}

		~Mesh() {
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);
		}

		void defaultSetup();
		
		void draw(const dlb::ShaderProgram& sp);

	private:
		std::vector<Vertex> vertices_;
		std::vector<uint> indices_;
		dlb::Texture2DGroup textures_;

		uint VAO, VBO, EBO;
	};
};