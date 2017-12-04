#version 330
          
smooth in vec2 uv;

uniform sampler2D gSampler; 

out vec4 frag_color; 

void main(void) 
{ 
  frag_color = texture2D(gSampler, uv.xy);
} 
