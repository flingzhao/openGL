#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}

#type fragment
#version 330 core
// out vec4 FragColor;

void main()
{             
    // gl_FragDepth = gl_FragCoord.z;
    // FragColor = vec4(1.0, 0.3, 0.2, 1.0);
}