#include "simple_render_system.hpp"

//libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>



namespace lve
{
	struct SimplePushConstantData
	{
		glm::mat4 transform{ 1.f };
		alignas(16) glm::vec3 color;
	};

	SimpleRenderSystem::SimpleRenderSystem(LveDevice& device, VkRenderPass renderPass)
		: lveDevice{device}
	{
		createPipelineLayout();
		createPipeline(renderPass);
	}
	SimpleRenderSystem::~SimpleRenderSystem()
	{
		vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
	}

	void SimpleRenderSystem::createPipelineLayout()
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
		if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create pipeline layout!");
		}
	}

	void SimpleRenderSystem::createPipeline(VkRenderPass renderPass)
	{
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		PipelineConfigInfo pipelineConfig{};
		LvePipeline::defaultPipelineConfigInfo
		(
			pipelineConfig
		);
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;
		lvePipeline = std::make_unique<LvePipeline>
			(
				lveDevice,
				".\\src\\shaders\\simple_shader.vert.spv",
				".\\src\\shaders\\simple_shader.frag.spv",
				pipelineConfig
			);
	}


	void SimpleRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<LveGameObject>& gameObjects)
	{
		lvePipeline->bind(commandBuffer);

		for (auto& obj : gameObjects)
		{
			obj.tranform.rotation.y = glm::mod(obj.tranform.rotation.y + 0.01f, glm::two_pi<float>());
			obj.tranform.rotation.x = glm::mod(obj.tranform.rotation.x + 0.01f, glm::two_pi<float>());
			obj.tranform.rotation.z = glm::mod(obj.tranform.rotation.z + 0.01f, glm::two_pi<float>());


			SimplePushConstantData push{};
			push.color = obj.color;
			push.transform = obj.tranform.mat4();

			vkCmdPushConstants(
				commandBuffer,
				pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(SimplePushConstantData),
				&push);
			obj.model->bind(commandBuffer);
			obj.model->draw(commandBuffer);
		}
	}

}