#version 150

in vec3 position;
in vec2 texcoord;

out vec2 _texcoord;
out vec3 _coord;

uniform mat4 MVPMatrix;


void main()
{
    gl_Position = MVPMatrix * vec4(position, 1);
    _texcoord = texcoord;
    _coord = vec3(gl_Position.x, gl_Position.y, gl_Position.z);
}
