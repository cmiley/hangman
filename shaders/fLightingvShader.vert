#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_uv;

out vec3 fN;
out vec3 fE;
out vec3 fL;
out vec2 uv;

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

void main()
{
    vec3 pos = (modelMatrix*vec4(v_position, 1.0)).xyz;
    fN = vec3(modelMatrix * vec4(v_normal, 0.0));
    fE = -pos;
    fL = LightPosition.xyz - pos;
    uv = v_uv;

    if( LightPosition.w != 0.0 ) {
	    fL = LightPosition.xyz - pos;
    }

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(v_position, 1.0);
}
