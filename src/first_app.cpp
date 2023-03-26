#include "first_app.hpp"
#include "systems/simple_render_system.hpp"
#include "systems/point_light_system.hpp"
#include "lve_camera.h"
#include "keyboard_movement_controller.h"
#include "lve_buffer.h"

//libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <chrono>

namespace lve
{

	struct GlobalUbo
	{
		glm::mat4 projection{ 1.f };
		glm::mat4 view{ 1.f };
		glm::vec4 ambientLightColor{ 1.f, 1.f, 1.f, 0.02f };
		glm::vec3 lightPosition{ -1.f, -1.f, -1.f };
		alignas(16) glm::vec4 lightColor{ 1.f, 1.f, 1.f, 0.7f };
	};

	FirstApp::FirstApp()
	{
		globalPool = LveDescriptorPool::Builder(lveDevice)
			.setMaxSets(LveSwapChain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, LveSwapChain::MAX_FRAMES_IN_FLIGHT)
			.build();
		loadGameObjects();
	}
	FirstApp::~FirstApp()
	{
	}
	void FirstApp::run()
	{
		std::vector<std::unique_ptr<LveBuffer>> uboBuffers(LveSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < uboBuffers.size(); i++)
		{
			uboBuffers[i] = std::make_unique<LveBuffer>
				(
					lveDevice,
					sizeof(GlobalUbo),
					1,
					VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
					VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
				);
			uboBuffers[i]->map();
		}

		auto globalSetLayout = LveDescriptorSetLayout::Builder(lveDevice)
			.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)
			.build();

		std::vector<VkDescriptorSet> globalDescriptorSets(LveSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < globalDescriptorSets.size(); i++)
		{
			auto bufferInfo = uboBuffers[i]->descriptorInfo();
			LveDescriptorWriter(*globalSetLayout, *globalPool)
				.writeBuffer(0, &bufferInfo)
				.build(globalDescriptorSets[i]);
		}

		SimpleRenderSystem simpleRenderSystem{ 
			lveDevice, 
			lveRenderer.getSwapChainRenderPass(), 
			globalSetLayout->getDescriptorSetLayout()
		};
		PointLightSystem pointLightSystem{
			lveDevice,
			lveRenderer.getSwapChainRenderPass(),
			globalSetLayout->getDescriptorSetLayout()
		};
        LveCamera camera{};

        camera.setViewTarget(glm::vec3(5.f, -3.f, -1.3f), glm::vec3(0.f, 0.f, 1.5f));

        auto viewerObject = LveGameObject::createGameObject();
		viewerObject.tranform.translation = { -1.f, -1.f, -1.f };
        KeyboardMovementController cameraController{};

        /*glfwSetInputMode(lveWindow.getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(lveWindow.getGLFWWindow(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);*/

        auto currentTime = std::chrono::high_resolution_clock::now();
		
		while (!lveWindow.shouldClose())
		{
			glfwPollEvents();
            
            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            cameraController.moveInPlaneXZ(lveWindow.getGLFWWindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.tranform.translation, viewerObject.tranform.rotation);

            float aspect = lveRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);
			if (auto commandBuffer = lveRenderer.beginFrame())
			{
				int frameIndex = lveRenderer.getFrameIndex();
				FrameInfo frameInfo{
					frameIndex, 
					frameTime, 
					commandBuffer, 
					camera, 
					globalDescriptorSets[frameIndex],
					gameObjects
				};
				GlobalUbo ubo{};
				ubo.projection = camera.getProjection();
				ubo.view = camera.getView();
				uboBuffers[frameIndex]->writeToBuffer(&ubo);
				uboBuffers[frameIndex]->flush();

				lveRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(frameInfo);
				pointLightSystem.render(frameInfo);
				lveRenderer.endSwapChainRenderPass(commandBuffer);
				lveRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(lveDevice.device());
	}


	void FirstApp::loadGameObjects()
	{
        std::shared_ptr<LveModel> lveModel = LveModel::createModelFromFile(lveDevice, "D:\\dev\\VulkanTurorial\\models\\smooth_vase.obj");

        auto squishedVase = LveGameObject::createGameObject();
		squishedVase.model = lveModel;
		squishedVase.tranform.translation = { 0.f, 0.5f, 0.0f };
		squishedVase.tranform.scale = { 1.f, 1.f, 1.f };

        gameObjects.emplace(squishedVase.getId(), std::move(squishedVase));

		lveModel = LveModel::createModelFromFile(lveDevice, "D:\\dev\\VulkanTurorial\\models\\flat_vase.obj");
		auto normalVase = LveGameObject::createGameObject();
		normalVase.model = lveModel;
		normalVase.tranform.translation = { 1.f, .5f, 0.0f };
		normalVase.tranform.scale = { 1.f, 1.f, 1.f };

		gameObjects.emplace(normalVase.getId(), std::move(normalVase));

		lveModel = LveModel::createModelFromFile(lveDevice, "D:\\dev\\VulkanTurorial\\models\\quad.obj");
		auto floor = LveGameObject::createGameObject();
		floor.model = lveModel;
		floor.tranform.translation = { 0.f, .5f, 0.f };
		floor.tranform.scale = glm::vec3(5.f);

		gameObjects.emplace(floor.getId(), std::move(floor));
	}

}