#include "keyboard_movement_controller.h"

namespace lve
{
	
	void KeyboardMovementController::moveInPlaneXZ(GLFWwindow* window, float dt, LveGameObject& gameObject)
	{
		//with mouse camera control *probably very bad and inefficient
		/*double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		if(!inputNormal) glfwSetCursorPos(window, 400, 300);
		horizontalAngle = lookSpeed * dt * float(400 - xpos);
		verticalAngle = lookSpeed * dt * float(300 - ypos);
		glm::vec3 rotate(
			verticalAngle,
			-horizontalAngle,
			0.f
		);
		gameObject.tranform.rotation += rotate;*/

		glm::vec3 rotate{0.f};
		if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate.y += 1.f;
		if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.f;
		if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate.x += 1.f;
		if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate.x -= 1.f;
		if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
			gameObject.tranform.rotation += lookSpeed * dt * glm::normalize(rotate);

		gameObject.tranform.rotation.x = glm::clamp(gameObject.tranform.rotation.x, -1.5f, 1.5f);
		gameObject.tranform.rotation.y = glm::mod(gameObject.tranform.rotation.y, glm::two_pi<float>());

		float yaw = gameObject.tranform.rotation.y;

		const glm::vec3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
		const glm::vec3 rightDir{ forwardDir.z, 0.f, -forwardDir.x };
		const glm::vec3 upDir{ 0.f, 1.f, 0.f };

		glm::vec3 moveDir{ 0.f };
		if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
		if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
		if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
		if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
		if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir += upDir;
		if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir -= upDir;

		if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon())
			gameObject.tranform.translation += moveSpeed * dt * glm::normalize(moveDir);

		/*if (glfwGetKey(window, keys.escape) == GLFW_PRESS)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			inputNormal = !inputNormal;
		}*/
	}
}