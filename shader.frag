#version 150

in vec2 _texcoord;
in vec3 _coord;

out vec4 o_color;

uniform sampler2D Tex;

void main()
{
    float fog_scalar = 1.0 - length(_coord) / 18;
    if (fog_scalar < 0)
        discard;
    o_color = fog_scalar * texture(Tex, _texcoord);
    o_color.a = 1;
}
