#pragma once

/*
* A level contains the map where the user and entities reside
*/

#include <iostream>
#include <vector>
#include <format>

#include "Application.hpp"

namespace ecs {


	struct Components {
		struct {
			glm::vec3 acceleration;
			glm::vec3 velocity;
			glm::vec3 position;
		} movement;
	};

	struct Entity {
		uint id;

		/* Model component */
		uint model_id;
		uint shader_id;

		bool dead;
	};

	class EntityPool {
	public:
		EntityPool(uint initial_size = 16)
		:entities_{},
		dead_entities_{} {
			
		}

	public:

		const std::vector<Entity>& getEntities() {
			return entities_;
		}

		uint newEntity(glm::vec3 pos0, glm::vec3 vel0, glm::vec3 accel0, uint shader_id, uint model_id) {
			auto& context = dlb::ApplicationSingleton::getInstance();

			uint id = entities_.size();

			if (dead_entities_.size() > 0) {
				id = dead_entities_.back();
				dead_entities_.pop_back();

				Entity& ent = entities_[id];
				ent.model_id = model_id;
				ent.shader_id = shader_id;
				ent.dead = false;

				components_[id].movement.acceleration = accel0;
				components_[id].movement.velocity = vel0;
				components_[id].movement.position = pos0;

				return id;
			}

			Entity ent = {
				.id = id,
				.model_id = model_id,
				.shader_id = shader_id,
				.dead = false,
			};
			entities_.push_back(ent);

			Components comp{};
			comp.movement.acceleration = accel0;
			comp.movement.velocity = vel0;
			comp.movement.position = pos0;
			components_.push_back(comp);

			assert(components_.size() == entities_.size());

			return entities_.size() - 1;
		}

		void kill(uint id) {
			if (id >= entities_.size())
				return;

			else {
				entities_[id].dead = true;
				dead_entities_.push_back(id);
			}
		}

		void iterate() {
			auto& context = dlb::ApplicationSingleton::getInstance();

			if (context.getPauseEcs())
				return;

			for (int i = 0; i < entities_.size(); i++) {
				auto& entity = entities_[i];

				if (entity.dead)
					continue;

				/*
				* Movement
				*/
				float delta_time = context.getDeltaTime();

				auto& mov = components_[entity.id].movement;

				mov.velocity += mov.acceleration * delta_time;
				mov.position += mov.velocity * delta_time;

				auto& model = context.getModel(entity.model_id);

				glm::vec3 bb_color = glm::vec3(0.0f);

				for (int j = 0; j < entities_.size(); j++) {
					if (i == j)
						continue;
					auto& current_model = context.getModel(entities_[j].model_id);
					auto& their_position = components_[entities_[j].id].movement.position;
					if (model.AABBTest(current_model, mov.position, their_position))
						bb_color.r = 1.0f;
					else
						bb_color.g = 1.0f;
				}

				model.translate(mov.position);

				auto trans = glm::mat4(1.0F);
				trans = glm::translate(trans, mov.position);

				model.draw(context.getShader(entity.shader_id), trans, bb_color);
			}
		}

	private:
		std::vector<Entity> entities_;
		std::vector<Components> components_;
		std::vector<uint> dead_entities_;
	};

}