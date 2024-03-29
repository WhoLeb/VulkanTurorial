#pragma once

#include "lve_model.hpp"

#include <glm/gtc/matrix_transform.hpp>

//std
#include <memory>
#include <unordered_map>

namespace lve
{
	struct TransformComponent
	{
		glm::vec3 translation{};
		glm::vec3 scale{1.f, 1.f, 1.f};
		glm::vec3 rotation{};

		glm::mat4 mat4();
		glm::mat3 normalMatrix();
	};

	class LveGameObject
	{
	public:
		using id_t = unsigned int;
		using Map = std::unordered_map<id_t, LveGameObject>;

		static LveGameObject createGameObject()
		{
			static id_t currentId = 0;
			return LveGameObject{ currentId++ };
		}

		LveGameObject(const LveGameObject&) = delete;
		LveGameObject &operator=(const LveGameObject&) = delete;
		LveGameObject(LveGameObject&&) = default;
		LveGameObject& operator=(LveGameObject&&) = default;

		const id_t getId() { return id; }

		std::shared_ptr<LveModel> model{};
		glm::vec3 color{};
		TransformComponent tranform{};

	private:
	
		LveGameObject(id_t objId) : id{ objId } {}

	private:
		id_t id;
	};
}

