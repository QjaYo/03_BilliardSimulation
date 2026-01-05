#version 330 core

#define MAX_LIGHTS 2

#define MAX_TEXTURE_DIFFUSE 2
#define MAX_TEXTURE_SPECULAR 2
#define MAX_TEXTURE_NORMAL 2
#define MAX_TEXTURE_HEIGHT 2

struct Material
{
  vec4 color;
  vec3 ka;
  vec3 kd;
  vec3 ks;
  float shininess;
};

struct Light
{
  vec4 position;
  vec4 color;
  float intensity;
};

in vec4 fPosition;
in vec4 fColor;
in vec3 fNormal;
in float fHeight;
in vec2 fTexCoords;

out vec4 FragColor;

// global uniforms
uniform vec3 cameraPosition;
// light uniforms
uniform int n_lights;
uniform Light lights[MAX_LIGHTS];
// object uniforms
uniform Material material;
uniform int n_TEXTURE_DIFFUSE;
uniform int n_TEXTURE_SPECULAR;
uniform int n_TEXTURE_NORMAL;
uniform int n_TEXTURE_HEIGHT;
uniform sampler2D TEXTURE_DIFFUSE[MAX_TEXTURE_DIFFUSE];
uniform sampler2D TEXTURE_SPECULAR[MAX_TEXTURE_SPECULAR];
uniform sampler2D TEXTURE_NORMAL[MAX_TEXTURE_NORMAL];
uniform sampler2D TEXTURE_HEIGHT[MAX_TEXTURE_HEIGHT];
uniform bool ACTIVATE_TEXTURE_DIFFUSE;
uniform bool ACTIVATE_TEXTURE_SPECULAR;
uniform bool ACTIVATE_TEXTURE_NORMAL;
uniform bool ACTIVATE_TEXTURE_HEIGHT;

vec3 getDiffuse()
{
  if (ACTIVATE_TEXTURE_DIFFUSE == false || n_TEXTURE_DIFFUSE == 0)
    return material.color.rgb;

  // average processing
  vec3 sumColor = vec3(0.0);
  for (int i=0; i<n_TEXTURE_DIFFUSE; i++)
  {
    sumColor += texture(TEXTURE_DIFFUSE[i], fTexCoords).rgb;
  }
  return sumColor / float(n_TEXTURE_DIFFUSE);
}

vec3 getSpecular()
{
  if (!ACTIVATE_TEXTURE_SPECULAR || n_TEXTURE_SPECULAR == 0)
    return material.ks;

  // average processing
  vec3 sumColor = vec3(0.0);
  for (int i=0; i<n_TEXTURE_SPECULAR; i++)
  {
    sumColor += texture(TEXTURE_SPECULAR[i], fTexCoords).rgb;
  }
  return sumColor / float(n_TEXTURE_SPECULAR);
}

vec3 getNormal()
{
  if (!ACTIVATE_TEXTURE_NORMAL || n_TEXTURE_NORMAL == 0)
    return normalize(fNormal);
  
  // average processing
  vec3 sumNormal = vec3(0.0);
  for (int i=0; i<n_TEXTURE_NORMAL; i++)
  {
    vec3 n = texture(TEXTURE_NORMAL[i], fTexCoords).rgb;
    sumNormal += normalize(n * 2.0 - 1.0); // 0~1 -> -1~1
  }
  return normalize(sumNormal / float(n_TEXTURE_NORMAL));
}

float getHeight()
{
  if (!ACTIVATE_TEXTURE_HEIGHT || n_TEXTURE_HEIGHT == 0)
    return fHeight;
  
  // average processing
  float sumHeight = 0.0;
  for (int i=0; i<n_TEXTURE_HEIGHT; i++)
  {
    sumHeight += texture(TEXTURE_HEIGHT[i], fTexCoords).r;
  }
  return sumHeight / float(n_TEXTURE_HEIGHT);
}

void main()
{
  vec3 cameraFront = normalize(fPosition.xyz - cameraPosition);

  vec3 lightDir;
  vec3 light_coef;

  vec3 ambient_coef = material.ka;
  vec3 diffuse_coef = getDiffuse();
  vec3 specular_coef = getSpecular();
  vec3 normal = getNormal();
  
  vec3 ambient = ambient_coef * diffuse_coef * vec3(1.0);
  vec3 sumColor = ambient;

  for (int i=0; i<n_lights; i++)
  {
    lightDir = normalize(lights[i].position.xyz - fPosition.xyz);
    light_coef = lights[i].color.xyz * lights[i].intensity;

    vec3 diffuse = diffuse_coef * light_coef * max(dot(normal, lightDir), 0.0);
    vec3 specular = specular_coef * light_coef * pow(max(dot(reflect(-lightDir, normal), -cameraFront), 0.0), material.shininess);

    sumColor += diffuse + specular;
  }

  FragColor = vec4(sumColor, 1.0);
}