#pragma once

#include "lve_window.hpp"
#include "lve_device.hpp"
#include "lve_game_object.hpp"
#include "lve_renderer.hpp"
#include "lve_descriptors.h"

#include <stdexcept>
#include <memory>
#include <vector>
#include <array>

#include "glm/glm.hpp"

namespace lve
{
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;

		void run();

	private:

		void loadGameObjects();

	private:
		LveWindow lveWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };
		LveDevice lveDevice{ lveWindow };
		LveRenderer lveRenderer{ lveWindow, lveDevice };
		
		std::unique_ptr<LveDescriptorPool> globalPool{};
		LveGameObject::Map gameObjects;
	};
}

