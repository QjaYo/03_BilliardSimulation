#include "callback.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "constant.h"

extern bool b_play;
extern glm::vec3 cameraPosition;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;
extern float fovy;
extern float pitch;
extern float yaw;
extern float roll;
extern float cameraSpeed;

static bool wasSpaceKeyPressed = false;
static bool isMouseRightDragging = false;
static double lastX = 400.0, lastY = 300.0;
static float rollSpeed = 1.0f;

void processInput(GLFWwindow *window)
{
  // ESC
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  // SPACE
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
  {
    if (!wasSpaceKeyPressed)
    {
      b_play = !b_play;
      wasSpaceKeyPressed = true;
    }
  }
  else
    wasSpaceKeyPressed = false;

  // Move Camera
  // W: Move Front
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    cameraPosition += cameraSpeed * glm::normalize(cameraFront);
  }
  // S: Move Back
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    cameraPosition -= cameraSpeed * glm::normalize(cameraFront);
  }
  // A: Move Left
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    //glm::mat4 rollMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(roll), cameraFront);
    //cameraUp = glm::normalize(glm::vec3(rollMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
    glm::vec3 leftDirection = glm::normalize(glm::cross(cameraUp, cameraFront));
    cameraPosition += cameraSpeed * leftDirection;
  }
  // D: Move Right
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    //glm::mat4 rollMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(roll), cameraFront);
    //cameraUp = glm::normalize(glm::vec3(rollMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
    glm::vec3 rightDirection = -glm::normalize(glm::cross(cameraUp, cameraFront));
    cameraPosition += cameraSpeed * rightDirection;
  }
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
  {
    cameraPosition -= cameraSpeed * glm::normalize(cameraUp);
  }
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
  {
    cameraPosition += cameraSpeed * glm::normalize(cameraUp);
  }

  // Roll Camera
  if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
  {
    roll += rollSpeed;
    glm::mat4 rollMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(roll), cameraFront);
    cameraUp = glm::normalize(glm::vec3(rollMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
  }
  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
  {
    roll -= rollSpeed;
    glm::mat4 rollMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(roll), cameraFront);
    cameraUp = glm::normalize(glm::vec3(rollMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
  }

  // Mouse Right Drag
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
  {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if (!isMouseRightDragging)
    {
      lastX = xpos;
      lastY = ypos;
      isMouseRightDragging = true;
    }

    float xoffset = (float)(xpos - lastX);
    float yoffset = (float)(-(ypos - lastY)); // Y축은 아래가 플러스이므로 반대로 계산
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.15f;
    yaw += sensitivity * xoffset;
    pitch += sensitivity * yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraFront = glm::normalize(direction);
  }
  else
  {
    isMouseRightDragging = false;
  }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
  float sensitivity = 0.4f;
  fovy -= sensitivity * (float)yoffset;

  if (fovy < 1.0f)
    fovy = 1.0f;
  if (fovy >= 90.0f)
    fovy = 90.0f - EPSILON;
}