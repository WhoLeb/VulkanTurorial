#include "first_app.hpp"
#include "simple_render_system.hpp"
#include "lve_camera.h"
#include "keyboard_movement_controller.h"

//libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <chrono>

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
        LveCamera camera{};

        camera.setViewTarget(glm::vec3(5.f, -3.f, -1.3f), glm::vec3(0.f, 0.f, 1.5f));

        auto viewerObject = LveGameObject::createGameObject();
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
				lveRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
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
		squishedVase.tranform.translation = { 0.f, 0.5f, 1.5f };
		squishedVase.tranform.scale = { 3.f, 1.f, 1.f };

        gameObjects.push_back(std::move(squishedVase));

		auto normalVase = LveGameObject::createGameObject();
		normalVase.model = lveModel;
		normalVase.tranform.translation = { .6f, 0.5f, 1.5f };
		normalVase.tranform.scale = { 1.f, 1.f, 1.f };

		gameObjects.push_back(std::move(normalVase));
	}

}