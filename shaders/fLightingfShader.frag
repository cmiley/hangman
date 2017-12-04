#version 330

in vec3 fN;
in vec3 fL;
in vec3 fE;
in vec2 uv;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform float Shininess;
uniform sampler2D gSampler;

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
    vec3 N = normalize(fN);
    vec3 E = normalize(fE);
    vec3 L = normalize(fL);
    vec4 frag_tex = texture2D(gSampler, uv.xy);

    vec3 H = normalize( L + E );   
    vec4 ambient = AmbientProduct;

    float Kd = max(dot(L, N), 0.0);
    vec4 diffuse = Kd*DiffuseProduct;
    
    float Ks = pow(max(dot(N, H), 0.0), Shininess);
    vec4 specular = Ks*SpecularProduct;

    if( dot(L, N) < 0.0 ) 
	specular = vec4(0.0, 0.0, 0.0, 1.0);

    gl_FragColor = ambient + diffuse + specular;
    gl_FragColor.a = 1.0;
    gl_FragColor.r = gl_FragColor.r * frag_tex.r;
    gl_FragColor.g = gl_FragColor.g * frag_tex.g;
    gl_FragColor.b = gl_FragColor.b * frag_tex.b;
}
