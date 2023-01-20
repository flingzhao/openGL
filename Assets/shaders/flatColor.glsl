#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;
layout (location = 2) in vec2 aTexCoord;

out vec4 vertexColor;
out vec2 TexCoord;

void main()
{
	gl_Position = vec4(a_Position, 1.0);
	vertexColor = vec4(a_Color, 1.0);
	TexCoord = aTexCoord;
}

#type fragment
#version 330 core
out vec4 FragColor;

in vec4 vertexColor;
in vec2 TexCoord;

uniform vec4 ourColor;
uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
	FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.2);
} 