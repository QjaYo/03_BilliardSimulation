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

// settings
GLFWwindow *window;
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

// Materials
Material *redBilliardBallMaterial;
Material *whiteLightMaterial;
Material *groundMaterial;

// Function Declarations
GLFWwindow *initOpenGL(const std::string &title, int width, int height);
void initCallbacks();
void initShaders();
void initMeshes();
void initMaterials();
void initObjects();

void update();
void updateTime();
void checkCollisions();

void renderScene();
void drawStaticScene();
void drawDynamicScene();
void drawUI();

void setUniforms_global(Shader &shader);
void setUniforms_light(Shader &shader);

int main()
{
  window = initOpenGL("OpenGL Project", SCR_WIDTH, SCR_HEIGHT);
  if (window == nullptr)
    return -1;
  glEnable(GL_DEPTH_TEST);

  initCallbacks();
  initShaders();
  initMeshes();
  initMaterials();
  initObjects();

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);
    update();
    renderScene();

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


GLFWwindow *initOpenGL(const std::string &title, int width, int height)
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return nullptr;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return nullptr;
  }

  return window;
}

void initCallbacks()
{
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetScrollCallback(window, scroll_callback);
}

void initShaders()
{
  phongShader = new Shader(SHADER_PATH "phong_vshader.glsl", SHADER_PATH "phong_fshader.glsl");
  lightShader = new Shader(SHADER_PATH "light_vshader.glsl", SHADER_PATH "light_fshader.glsl");
  cubemapShader = new Shader(SHADER_PATH "cubemap_vshader.glsl", SHADER_PATH "cubemap_fshader.glsl");
}

void initMeshes()
{
  sphereMesh = ShapeGenerator::createSphere();
  cubeMesh = ShapeGenerator::createCube();
  quadMesh = ShapeGenerator::createQuad();
}

void initMaterials()
{
  redBilliardBallMaterial = new Material(
    glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
    glm::vec3(0.1f),
    glm::vec3(0.5f),
    glm::vec3(0.5f),
    72.0f
  );

  whiteLightMaterial = new Material(
    glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
    glm::vec3(0.1f),
    glm::vec3(0.5f),
    glm::vec3(0.5f),
    72.0f
  );

  groundMaterial = new Material(
    glm::vec4(0.4f, 0.6f, 0.3f, 1.0f),
    glm::vec3(0.1f),
    glm::vec3(0.5f),
    glm::vec3(0.0f),
    72.0f
  );
}

void initObjects()
{
  // Set Lights
  lights.push_back(
    Light(
      Object(
        TransformData { glm::vec3(0.0f, 30.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f) },
        RenderData { lightShader, sphereMesh, whiteLightMaterial },
        PhysicsData {}
      ),
      1.0f
    )
  );

  // Ground
  ground = Object(
    TransformData { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(100.0f, 100.0f, 0.5f) },
    RenderData { phongShader, cubeMesh, groundMaterial },
    PhysicsData {}
  );
}


void updateTime()
{
  float current_time = static_cast<float>(glfwGetTime());
  dt = current_time - last_time;
  last_time = current_time;
  if (b_play)
    accumulated_time += dt;
}

void update()
{
  updateTime();
}

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
    shader.setFloat("lightRadius[" + std::to_string(i) + "]", lights[i].object.transform.scale.x);
    shader.setFloat("attenuation_exp[" + std::to_string(i) + "]", 1.0);
  }
}

void drawStaticScene()
{
  // Ground
  phongShader->use();
  setUniforms_global(*phongShader);
  setUniforms_light(*phongShader);
  //sphere->draw(phongShader);
  //cube->draw(phongShader);
  ground.draw(*phongShader);

  // Lights
  static std::vector<bool> b_drawLights = { true };
  b_drawLights[0] = false;
  for (int i = 0; i < lights.size(); i++)
  {
    if (b_drawLights[i])
    {
      lightShader->use();
      setUniforms_global(*lightShader);
      setUniforms_light(*lightShader);
      for (int i = 0; i < lights.size(); i++)
        lights[i].object.draw(*lightShader);
    }
  }
}

void drawDynamicScene()
{
  return;
}

void drawUI()
{
  return;
}

void renderScene()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  drawStaticScene();
  drawDynamicScene();
  drawUI();
}