#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
#include <cmath>

#include "vec3.h"
#include "emfield.h"
#include "camera.h"

const float window_width = 1080;
const float window_height = 920;

const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;    // Position of vertex
layout(location = 1) in vec3 aField;  // Electric/magnetic field vector

out vec3 color;  // Output color to fragment shader

uniform mat4 view;
uniform mat4 projection;

void main() {
    // Compute the magnitude (strength) of the field vector
    float fieldStrength = length(aField);  // Magnitude of field

    // Normalize the field vector to get the direction (this is used in some cases)
    vec3 normalizedField = normalize(aField);

    // Map field strength to a color (you can tweak the range or scale)
    // Here, I'm mapping fieldStrength to a color scale. You can customize this.
    color = vec3(fieldStrength, 0.0, 1.0 - fieldStrength);  // Blue to Red for example

    // Transform the position for rendering
    gl_Position = projection * view * vec4(aPos, 1.0);  // Apply transformations
})";


const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
in vec3 color;  // Color input from vertex shader

void main() {
    FragColor = vec4(color, 1.0);  // Set color with full opacity
})";



// Shader Functions
GLuint compileShader(const char* source, GLenum shaderType);
GLuint createShaderProgram(void);
void drawField(GLuint shaderProgram, const std::vector<Vec3>& fields);

Camera camera(glm::vec3(50.0f, 50.0f, 150.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    camera.mouseCallback(window, xpos, ypos);
}

// Main OpenGL Program
int main() {
    // GLFW initialization
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(1080, 920, "Electromagnetic Field Simulator", nullptr, nullptr);
    if (!window) return -1;
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    // Simulator Setup
    EMFields simulator(100, 100, 100, 1);
    simulator.applyPointChargeElectricField(Vec3(50, 50, 50), 1e-6); // Apply point charge

    GLuint shaderProgram = createShaderProgram();
    // Time step for simulation
    const float deltaTime = 0.005f;  // Fixed time step for simulation

    glEnable(GL_DEPTH_TEST);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        camera.processInput(window, deltaTime);
        glfwSetCursorPosCallback(window, mouseCallback);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        
        // Set the viewport size
        glViewport(0, 0, width, height);

        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glEnable(GL_DEPTH_TEST);  // Enable depth testing

        // Projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);

        // View matrix
        glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);

        // Pass the view and projection matrices to the shader
        GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
        GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        simulator.updateFields(deltaTime);
        drawField(shaderProgram, simulator.electric_field);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// Shader Functions Implementation
GLuint compileShader(const char* source, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader Compilation Failed: " << infoLog << std::endl;
    }
    return shader;
}

GLuint createShaderProgram(void) {
    GLuint vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Program Linking Failed: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void drawField(GLuint shaderProgram, const std::vector<Vec3>& fields) {
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> fieldVectors;

    for (const auto& field : fields) {
        // Each point consists of a position and a field vector
        vertices.push_back(field.x);
        vertices.push_back(field.y);
        vertices.push_back(field.z);

        // Electric field vector to influence color
        fieldVectors.push_back(field.x);
        fieldVectors.push_back(field.y);
        fieldVectors.push_back(field.z);
    }

    GLuint VBO[2], VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(2, VBO); // We need two buffers, one for position and one for field vectors

    glBindVertexArray(VAO);

    // Position buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Field vector buffer (for color calculation)
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, fieldVectors.size() * sizeof(GLfloat), fieldVectors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, vertices.size() / 3);
    glBindVertexArray(0);
}
