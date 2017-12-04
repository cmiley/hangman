#version 330

layout (location = 0) in vec3 v_position; 
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_uv;

smooth out vec2 uv;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform mat4 modelMatrix, viewMatrix, projectionMatrix;
uniform float Shininess;
uniform vec4 LightPosition;

struct LightSource
{
    int Type;
    vec3 Position;
    vec3 Attenuation;
    vec3 Direction;
    vec3 Color;
};

uniform LightSource Light[4];

void main(void) 
{ 
  vec4 v = vec4(v_position, 1.0); 
  gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * v; 
  uv = v_uv;
} 
