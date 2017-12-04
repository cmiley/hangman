#version 330

in vec4 color;
in vec2 uv;

uniform sampler2D gSampler;

out vec4 frag_color;

void main(void)
{
    frag_color = texture2D(gSampler, uv.xy);
    frag_color.r = frag_color.r * color.r;
    frag_color.g = frag_color.g * color.g;
    frag_color.b = frag_color.b * color.b;
}
