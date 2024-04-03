#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_Texture;

out vec3 FragPos;  
out vec3 Normal;
out vec2 TexCoords;


uniform mat4 ModelMat;
uniform mat4 ViewMat;
uniform mat4 ProjectionMat;

void main()
{
    FragPos = vec3(ModelMat * vec4(a_Position, 1.0f));
    Normal = a_Normal; 
    TexCoords = a_Texture;

	gl_Position = ProjectionMat * ViewMat * ModelMat * vec4(a_Position, 1.0f);  
}

#type fragment
#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoords;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos; 
uniform Material material;
uniform Light light;


void main()
{
    // ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse =  light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),  material.shininess / 32);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));  

    vec3 result = (ambient + diffuse + specular);
	FragColor = vec4(result, 1.0f);
} 