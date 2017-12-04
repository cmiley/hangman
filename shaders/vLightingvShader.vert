#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_uv;

out vec4 color;
out vec2 uv;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform mat4 modelMatrix, viewMatrix, projectionMatrix;
uniform vec4 LightPosition;
uniform float Shininess;

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
    vec3 pos = (modelMatrix * vec4(v_position, 1.0)).xyz;
	vec4 totalSpecular = vec4(0.0);
	vec4 ambient = AmbientProduct;
	vec4 totalDiffuse = vec4(0.0);

    vec3 L = normalize( LightPosition.xyz - pos );
    vec3 E = normalize( -pos );
    vec3 H = normalize( L + E );

    vec3 N = normalize( modelMatrix*vec4(v_normal, 0.0) ).xyz;

    float Kd = max( dot(L, N), 0.0 );
    vec4  diffuse = Kd*DiffuseProduct;
    totalDiffuse = totalDiffuse + diffuse;
    float Ks = pow( max(dot(N, H), 0.0), Shininess );
    vec4  specular = Ks * SpecularProduct;
    if( dot(L, N) < 0.0 )  specular = vec4(0.0, 0.0, 0.0, 1.0);
    totalSpecular = totalSpecular + specular;

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(v_position, 1.0);

    color = ambient + totalDiffuse + totalSpecular;
    color.a = 1.0;

    uv = v_uv;
}
