#pragma once

#include "lve_pipeline.hpp"
#include "lve_device.hpp"
#include "lve_game_object.hpp"

#include <stdexcept>
#include <memory>
#include <vector>
#include <array>

#include "glm/glm.hpp"

namespace lve
{
	class SimpleRenderSystem
	{
	public:
		SimpleRenderSystem(LveDevice& device, VkRenderPass renderPass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<LveGameObject>& gameObjects);
	
	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

	private:
		LveDevice& lveDevice;
		std::unique_ptr<LvePipeline> lvePipeline;
		VkPipelineLayout pipelineLayout;
	};
}

