#define _CRT_SECURE_NO_WARNINGS
#define GLM_FORCE_SWIZZLE

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "callback.h"
#include "shader.h"
#include "shapeGenerator.h"
#include "mesh.h"
#include "vertex.h"
#include "material.h"
#include "constant.h"
#include "light.h"
#include "texture.h"
#include "object.h"

// setttings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Display
bool b_play = true;
float fovy = 45.0f;
float near = 0.1f;
float far = 1000.0f;

// Camera
float pitch = -30.0f;
float yaw = -90.0f;
float roll = 0.0f;
glm::vec3 cameraFront = glm::vec3(0.0f, sin(glm::radians(pitch)), -cos(glm::radians(pitch)));
glm::vec3 cameraPosition = glm::vec3(0.0f, 30.0f, 50.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Time variables
float dt = 0.0f;
float last_time = 0.0f;
float accumulated_time = 0.0f;

// Environments
Object ground;
std::vector<Light> lights;

// Shaders
Shader *phongShader;
Shader *lightShader;
Shader *cubemapShader;

// Meshes
Mesh *sphereMesh;
Mesh *cubeMesh;
Mesh *quadMesh;

// Functions
void setUniforms_global(Shader &shader)
{
  shader.use();

  // Matrices
  glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
  shader.setMat4("viewMatrix", viewMatrix);

  glm::mat4 projMatrix = glm::perspective(glm::radians(fovy), (float)SCR_WIDTH / (float)SCR_HEIGHT, near, far);
  shader.setMat4("projMatrix", projMatrix);

  // Camera
  shader.setVec3("cameraPosition", cameraPosition);
}

void setUniforms_light(Shader &shader)
{
  shader.use();
  shader.setInt("n_lights", (int)lights.size());

  for (int i = 0; i < lights.size(); i++)
  {
    std::string name = "lights[" + std::to_string(i) + "]";
    shader.setLight(name, lights[i]);
    shader.setFloat("lightRadius[" + std::to_string(i) + "]", lights[i].object.scale.x);
    shader.setFloat("attenuation_exp[" + std::to_string(i) + "]", 1.0);
  }
}

void setEnvironment()
{
  // Set Lights
  lights.push_back(
    Light(
      Object(
        glm::vec3(0.0f, 30.0f, 0.0f),
        glm::vec3(0.0f),
        glm::vec3(1.0f),
        Material(
          glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), // color: white
          glm::vec3(1.0f),
          glm::vec3(1.0f),
          glm::vec3(1.0f),
          0.0f
        ),
        sphereMesh
      ),
      0.5f
    )
  );

  // Ground
  ground = Object(
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(90.0f, 0.0f, 0.0f),
    glm::vec3(100.0f, 100.0f, 0.5f),
    Material(
      1.5f * glm::vec4(0.435f, 0.309f, 0.156f, 1.0f),   // Color
      glm::vec3(0.5f),                                  // Ambient
      glm::vec3(1.0f),                                  // Diffuse
      glm::vec3(0.2f),                                  // Specular
      1.0f
    ),
    cubeMesh
  );
}

void drawEnvironment()
{
  // Draw General Objects
  phongShader->use();
  setUniforms_global(*phongShader);
  setUniforms_light(*phongShader);
  //sphere->draw(phongShader);
  //cube->draw(phongShader);
  ground.draw(*phongShader);

  // Draw Lights
  lightShader->use();
  setUniforms_global(*lightShader);
  setUniforms_light(*lightShader);
  for (int i = 0; i < lights.size(); i++)
  {
    lights[i].object.draw(*lightShader);
  }
}


int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // register callbacks
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetScrollCallback(window, scroll_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // settings
  glEnable(GL_DEPTH_TEST);

  // Shaders
  phongShader = new Shader(SHADER_PATH "phong_vshader.glsl", SHADER_PATH "phong_fshader.glsl");
  lightShader = new Shader(SHADER_PATH "light_vshader.glsl", SHADER_PATH "light_fshader.glsl");
  cubemapShader = new Shader(SHADER_PATH "cubemap_vshader.glsl", SHADER_PATH "cubemap_fshader.glsl");

  // Meshes
  sphereMesh = ShapeGenerator::createSphere();
  cubeMesh = ShapeGenerator::createCube();
  quadMesh = ShapeGenerator::createQuad();

  setEnvironment();

  while (!glfwWindowShouldClose(window))
  {
    // Update time
    float current_time = static_cast<float>(glfwGetTime());
    dt = current_time - last_time;
    last_time = current_time;
    if (b_play)
      accumulated_time += dt;

    processInput(window);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawEnvironment();

    // draw something

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  delete phongShader;
  delete lightShader;
  delete cubemapShader;

  delete sphereMesh;
  delete cubeMesh;
  delete quadMesh;

  glfwTerminate();
  return 0;
}