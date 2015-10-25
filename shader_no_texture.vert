#version 150

uniform mat4 u_mvp_matrix;
in vec3 si_position;
out vec3 sp_position;

void main()
{
    gl_Position = u_mvp_matrix * vec4(si_position, 1);
    sp_position = vec3(gl_Position.x, gl_Position.y, gl_Position.z);
}
