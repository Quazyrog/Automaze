#version 150

in vec2 _texcoord;
in vec3 _coord;

out vec4 o_color;

uniform sampler2D Tex;

void main()
{
    float fog_scalar = 1.0 - length(_coord) / 18;
    float alpha;
    if (fog_scalar < 0)
        discard;
    o_color = texture(Tex, _texcoord);
    alpha = o_color.a;
    o_color = fog_scalar * o_color;
    o_color.a = alpha;
}
