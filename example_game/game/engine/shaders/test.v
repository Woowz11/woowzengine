#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform vec2 scale;
uniform float height;

out vec2 TexCoord;

void main()
{
	vec2 NewPosition = aPos.xy * scale;
	gl_Position = vec4(NewPosition, 0, 1);
	TexCoord = aTexCoord;
}

