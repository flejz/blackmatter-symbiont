#version 330 core
in vec3 fragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
  outColor = vec4(lightColor * objectColor, 1.0);
}
