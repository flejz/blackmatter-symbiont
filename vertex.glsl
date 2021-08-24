#version 330 core
layout (location = 0) in vec3 verCoords;
layout (location = 1) in vec2 verTexCoords;

out vec3 fragColor;
out vec2 fragTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  gl_Position = projection * view * model * vec4(verCoords, 1.0f);
  fragTexCoords = verTexCoords;
}
