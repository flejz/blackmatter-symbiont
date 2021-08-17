#version 330 core
layout (location = 0) in vec3 verCoords;
layout (location = 1) in vec3 verColor;
layout (location = 2) in vec2 verTexCoords;

out vec3 fragColor;
out vec2 fragTexCoords;

void main()
{
  gl_Position = vec4(verCoords, 1.0);
  fragColor = verColor;
  fragTexCoords = verTexCoords;
  
}
