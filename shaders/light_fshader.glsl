#version 330 core

#define MAX_LIGHTS 2

struct Light
{
  vec4 position;
  vec4 color;
  float intensity;
};

in vec4 fPosition;

out vec4 FragColor;

uniform int n_lights;
uniform Light lights[MAX_LIGHTS];
uniform float lightRadius[MAX_LIGHTS];
uniform float attenuation_exp[MAX_LIGHTS];

uniform vec3 cameraPosition;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main() 
{
    vec3 sumColor = vec3(0.0);
    // vec3 viewDir = normalize(fPosition.xyz - cameraPosition);

    // for (int i=0; i<n_lights; i++)
    // {
    //     vec3 camera2light = normalize(lights[i].position.xyz - cameraPosition);
    //     vec3 light2point = fPosition.xyz - lights[i].position.xyz;
    //     vec3 proj = dot(light2point, camera2light) * camera2light;
    //     float dist = length(light2point - proj);
        
    //     if (dist < lightRadius[i])
    //     {
    //         float attenuation_coef = pow(1.0 - (dist / lightRadius[i]), attenuation_exp[i]);
    //         sumColor += attenuation_coef * lights[i].intensity * lights[i].color.rgb;
    //     }
    // }
    sumColor = lights[0].color.rgb;
    FragColor = vec4(sumColor, 1.0);
}   