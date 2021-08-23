#version 330 core
in vec3 fragColor;
in vec2 fragTexCoords;

out vec4 outColor; 

uniform sampler2D uniTexture1;
uniform sampler2D uniTexture2;

void main()
{
  // rainbown
  // outColor = vec4(fragColor, 1.0);

  // rainbow box
  // outColor = texture(uniTexture1, fragTexCoords) * vec4(fragColor, 1.0);

  // box and smile
  outColor = mix(texture(uniTexture1, fragTexCoords), texture(uniTexture2, fragTexCoords), 0.4); 
}
