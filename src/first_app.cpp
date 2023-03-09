#include "first_app.hpp"
#include "simple_render_system.hpp"

//libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>



namespace lve
{

	FirstApp::FirstApp()
	{
		loadGameObjects();
	}
	FirstApp::~FirstApp()
	{
	}
	void FirstApp::run()
	{
		SimpleRenderSystem simpleRenderSystem{ lveDevice, lveRenderer.getSwapChainRenderPass() };
		while (!lveWindow.shouldClose())
		{
			glfwPollEvents();
			if (auto commandBuffer = lveRenderer.beginFrame())
			{
				lveRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
				lveRenderer.endSwapChainRenderPass(commandBuffer);
				lveRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(lveDevice.device());
	}

	void FirstApp::loadGameObjects()
	{
		std::vector<LveModel::Vertex> vertices
		{
			{{-0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
			{{-0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
		};

		auto lveModel = std::make_shared<LveModel>(lveDevice, vertices);
		auto triangle = LveGameObject::createGameObject();
		triangle.model = lveModel;
		triangle.color = { .1f, .8f, .1f };
		triangle.tranform2d.translation.x = .2f;
		triangle.tranform2d.scale = { 0.5f, 0.5f };
		triangle.tranform2d.rotation = .1f * glm::pi<float>();
		gameObjects.push_back(std::move(triangle));
	}

}