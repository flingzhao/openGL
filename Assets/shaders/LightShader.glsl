#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;

uniform mat4 ModelMat;
uniform mat4 ViewMat;
uniform mat4 ProjectionMat;


void main()
{
	gl_Position = ProjectionMat * ViewMat * ModelMat * vec4(a_Position, 1.0f);   
}

#type fragment
#version 330 core
out vec4 FragColor;


uniform vec4 ourColor;
uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
	// FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.2);
	FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
} 