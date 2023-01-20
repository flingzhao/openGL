#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 ModelMat;
uniform mat4 ViewMat;
uniform mat4 ProjectionMat;

out vec2 TexCoord;

void main()
{
	gl_Position = ProjectionMat * ViewMat * ModelMat * vec4(a_Position, 1.0f);   
	TexCoord = aTexCoord;
}

#type fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform vec4 ourColor;
uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
	// FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.2);
	FragColor = texture(texture0, TexCoord);
} 