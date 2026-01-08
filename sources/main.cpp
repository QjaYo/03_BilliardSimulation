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
#include "component.h"
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
float cameraSpeed = 0.4f;

//Objects
Object billiardTable;

// Components & Properities
Component _felt;
std::array<Component, 4> _rails;
Component _billiardTable;
std::vector<Component> _balls;

// Lights
std::vector<Light> lights;

// Time variables
float dt = 0.0f;
float last_time = 0.0f;
float accumulated_time = 0.0f;

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
Material *feltMaterial;
Material *railMaterial;

// Textures
Texture *skyboxTexture;

// Function Declarations
GLFWwindow *initOpenGL(const std::string &title, int width, int height);
void initCallbacks();
void initShaders();
void initMeshes();
void initMaterials();
void initObjects();
void initLights();
void initBilliardTable();
void initSkybox();

void update();
void updateTime();
void checkCollisions();

void renderScene();
void drawStaticScene();
void drawDynamicScene();
void drawUI();

void setUniforms_global(Shader &shader);
void setUniforms_light(Shader &shader);

void deleteResources();

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
  initSkybox();

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);
    update();
    renderScene();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  deleteResources();

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

  feltMaterial = new Material(
    glm::vec4(0.183f, 0.313f, 0.141f, 1.0f),
    glm::vec3(0.2f),
    glm::vec3(0.5f),
    glm::vec3(0.0f),
    72.0f
  );

  railMaterial = new Material(
    glm::vec4(0.345f, 0.156f, 0.06f, 1.0f),
    glm::vec3(0.2f),
    glm::vec3(0.5f),
    glm::vec3(0.0f),
    36.0f
  );
}

void initLights()
{
  // Set Lights
  lights.push_back(
    Light(
      Component(
        TransformData { glm::vec3(0.0f, 30.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f) },
        RenderData { lightShader, sphereMesh, whiteLightMaterial },
        PhysicsData {}
      ),
      1.0f
    )
  );
}

void initObjects()
{
  initLights();

  // Felts
  _felt = Component(
    TransformData { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(100.0f, 100.0f, 0.5f) },
    RenderData { phongShader, cubeMesh, feltMaterial },
    PhysicsData {}
  );

  // Rails
  float railThickness = 2.0f;
  float railDepth = 5.0f;
  for (auto i = 0; i < _rails.size(); i++)
  {
    float ang = i * 90.0f;
    float rad = glm::radians(ang);
    _rails[i] = Component(
      TransformData {
        glm::vec3((_felt.transform.scale.x + railThickness) / 2.0f * cos(rad), (_felt.transform.scale.y + railThickness) / 2.0f * sin(rad), 0.0f),
        glm::vec3(0.0f, 0.0f, ang),
        glm::vec3(railThickness, _felt.transform.scale.y + glm::abs(railThickness * 2.0f * cos(rad)), railDepth)
      },
      RenderData { phongShader, cubeMesh, railMaterial },
      PhysicsData {}
    );
  }

  // BilliardTable
  _billiardTable = Component(
    TransformData { glm::vec3(0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
    RenderData {},
    PhysicsData {}
  );
  _billiardTable.addChild(&_felt);
  for (auto i = 0; i < _rails.size(); i++)
    _billiardTable.addChild(&_rails[i]);

  billiardTable = Object("BilliardTable", &_billiardTable);

  // Balls
  int n_ball = 1;
  for (auto i = 0; i < n_ball; i++)
  {
    float radius = 1.0f;
    _balls.push_back(
      Component(
        TransformData { glm::vec3(0.0f, 0.0f, -(billiardTable.root->transform.scale.z / 2.0f + radius)), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(radius) },
        RenderData { phongShader, sphereMesh, redBilliardBallMaterial },
        PhysicsData { 1.0f, glm::vec3(0.2f, 0.3f, 0.2f), glm::vec3(0.0f), glm::vec3(0.0f), 0.0f }
      )
    );
    _billiardTable.addChild(&_balls[0]);
  }
}

void initSkybox()
{
  std::vector<std::string> faces {
    TEXTURE_PATH "skybox_universe.jpg",
    TEXTURE_PATH "skybox_universe.jpg",
    TEXTURE_PATH "skybox_universe.jpg",
    TEXTURE_PATH "skybox_universe.jpg",
    TEXTURE_PATH "skybox_universe.jpg",
    TEXTURE_PATH "skybox_universe.jpg"
  };
  skyboxTexture = new Texture("skybox_universe");
  skyboxTexture->loadCubeMap(faces);
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
    shader.setFloat("lightRadius[" + std::to_string(i) + "]", lights[i].component.transform.scale.x);
    shader.setFloat("attenuation_exp[" + std::to_string(i) + "]", 1.0);
  }
}

void drawStaticScene()
{
  // Lights
  lightShader->use();
  static std::vector<bool> b_drawLights = { true };
  b_drawLights[0] = false;
  for (int i = 0; i < lights.size(); i++)
  {
    if (b_drawLights[i])
    {
      setUniforms_global(*lightShader);
      setUniforms_light(*lightShader);
      for (int i = 0; i < lights.size(); i++)
        lights[i].component.draw(*lightShader, glm::mat4(1.0f));
    }
  }

  // Skybox
  cubemapShader->use();
  glDepthFunc(GL_LEQUAL);
  cubemapShader->use();
  setUniforms_global(*cubemapShader);
  if (skyboxTexture != nullptr)
  {
    int textureUnit = 0;
    skyboxTexture->bind(textureUnit);
    cubemapShader->setInt("cubemap", textureUnit);
  }
  if (cubeMesh != nullptr)
  {
    cubeMesh->draw(*cubemapShader);
  }
  glDepthFunc(GL_LESS);

}

void drawDynamicScene()
{
  phongShader->use();
  setUniforms_global(*phongShader);
  setUniforms_light(*phongShader);
  billiardTable.draw(*phongShader);

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

void deleteResources()
{

  delete phongShader;
  delete lightShader;
  delete cubemapShader;

  delete sphereMesh;
  delete cubeMesh;
  delete quadMesh;

  delete skyboxTexture;

  delete redBilliardBallMaterial;
  delete whiteLightMaterial;
  delete feltMaterial;
  delete railMaterial;
}