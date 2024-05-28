#version 330 core
out vec4 FragColor;

uniform vec4 color;
uniform sampler2D sprite;

in vec2 TexCoord;

void main()
{
    FragColor = texture(sprite, TexCoord) * color;
} 

