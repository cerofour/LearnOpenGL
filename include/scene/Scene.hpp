#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "Mesh.hpp"
#include "Types.hpp"

namespace scene {
	class Scene {
	public:
		Scene() {}

	public:
		void createPlane(const glm::vec3& position, const glm::vec3& normal, float width, float height);

	private:
		std::vector<uint> entities_;
		
		// used to draw structures like planes, cubes, etc, for level loading
		std::vector<BasicMesh> structures_;
	};
}