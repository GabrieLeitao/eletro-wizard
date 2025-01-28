#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
    Camera(glm::vec3 startPos, glm::vec3 startFront, glm::vec3 startUp);

    void processInput(GLFWwindow* window, float deltaTime);
    void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    glm::mat4 getViewMatrix() const;

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    float yaw;
    float pitch;
    bool firstMouse;
    float sensitivity;
    float speed;

private:
    float lastX, lastY;  // Mouse positions
};

#endif
