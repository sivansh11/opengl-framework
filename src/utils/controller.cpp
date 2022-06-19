#include "utils/controller.h"

void KeyboardMovementController::moveInPlaneXZ(GLFWwindow* window, float dt, glm::vec3 &transform_, glm::vec3 &rotate_)
{
    glm::vec3 rotate{0};

    if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate.y += 1; 
    if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate.y -= 1; 
    if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate.x += -1; 
    if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate.x -= -1; 

    if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
    {
        rotate_ += lookSpeed * dt * glm::normalize(rotate);
    }

    rotate_.x = glm::clamp(rotate_.x, -1.5f, 1.5f);
    rotate_.y = glm::mod(rotate_.y, glm::two_pi<float>());

    float yaw = rotate_.y;
    const glm::vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
    const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
    const glm::vec3 upDir{0.f, 1.f, 0.f};

    glm::vec3 moveDir{0.f};
    if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir; 
    if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
    if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir; 
    if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir; 
    if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir += upDir; 
    if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir -= upDir; 

    if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon())
    {
        transform_ += moveSpeed * dt * glm::normalize(moveDir);
    }
}

