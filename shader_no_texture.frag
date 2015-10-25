#version 150

uniform vec4 u_color;
in vec3 sp_position;
out vec4 so_color;

void main()
{
    float fog_scalar = 1.0 - length(sp_position) / 18;
    if (fog_scalar < 0)
        discard;
    so_color =  vec4(fog_scalar * vec3(0, 0, 1), 0.7);
}