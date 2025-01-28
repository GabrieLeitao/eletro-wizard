#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 startPos, glm::vec3 startFront, glm::vec3 startUp)
    : position(startPos), front(startFront), up(startUp), yaw(-90.0f), pitch(0.0f), firstMouse(true), sensitivity(0.1f), speed(1000.0f) {}

void Camera::processInput(GLFWwindow* window, float deltaTime) {
    float velocity = speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) position += front * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) position -= front * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) position -= glm::normalize(glm::cross(front, up)) * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) position += glm::normalize(glm::cross(front, up)) * velocity;
}

void Camera::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xOffset = xpos - lastX;
        float yOffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        xOffset *= sensitivity;
        yOffset *= sensitivity;

        yaw -= xOffset;
        pitch -= yOffset;

        // Constrain the pitch to avoid flipping
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(direction);
    }
}


glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}
