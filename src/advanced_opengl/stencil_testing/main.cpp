#include <iostream>
#include <string_view>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "../util.h"

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if(window == NULL)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    //     float cubeVertices[] = {
    //     // positions          // texture Coords
    //     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    //      0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    //      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    //     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    //     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    //      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    //      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    //     -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    //     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    //     -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //     -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //     -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    //      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //      0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    //     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //      0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    //      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    //      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    //     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    //     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    //      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //     -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    //     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    // };
    // float planeVertices[] = {
    //     // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
    //      5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    //     -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
    //     -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

    //      5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    //     -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
    //      5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    // };
    // // cube VAO
    // unsigned int cubeVAO, cubeVBO;
    // glGenVertexArrays(1, &cubeVAO);
    // glGenBuffers(1, &cubeVBO);
    // glBindVertexArray(cubeVAO);
    // glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // glBindVertexArray(0);
    // // plane VAO
    // unsigned int planeVAO, planeVBO;
    // glGenVertexArrays(1, &planeVAO);
    // glGenBuffers(1, &planeVBO);
    // glBindVertexArray(planeVAO);
    // glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // glBindVertexArray(0);

    // Shader shader("../../../Assets/shaders/CubesShader.glsl");
    // shader.Use();

	Shader Lightshader(std::string("../../../Assets/shaders/LightShader.glsl")), Objshader(std::string("../../../Assets/shaders/ObjectShader.glsl"));
	Lightshader.Use();

	glm::mat4 view = camera.GetViewMatrix();
    Lightshader.SetMat4("ViewMat", view);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    Lightshader.SetMat4("ProjectionMat", projection);

	Objshader.Use();

	glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
	Objshader.SetUniform3f("objectColor", objectColor);
	glm::vec3 lightPos(0.0f, 1.0f, 1.0f);
	Objshader.SetUniform3f("lightPos", lightPos);

	Objshader.SetMat4("ViewMat", view);
	Objshader.SetUniform3f("ViewPos", camera.GetCameraPosition());
    Objshader.SetMat4("ProjectionMat", projection);

	Objshader.SetVec3("material.ambient",  1.0f, 0.5f, 0.31f);
	Objshader.SetVec3("material.diffuse",  1.0f, 0.5f, 0.31f);
	Objshader.SetVec3("material.specular", 0.5f, 0.5f, 0.5f);
	Objshader.SetFloat("material.shininess", 32.0f);

	Objshader.SetVec3("light.ambient",  0.5f, 0.5f, 0.5f);
	Objshader.SetVec3("light.diffuse",  0.6f, 0.8f, 0.8f); // 将光照调暗了一些以搭配场景
	Objshader.SetVec3("light.specular", 1.0f, 1.0f, 1.0f); 


    float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    // world space positions of our cubes


	unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

	unsigned int LightVAO, ObjVAO;
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// bind EBO after binding VBO
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &ObjVAO);
	glBindVertexArray(ObjVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenVertexArrays(1, &LightVAO);
	glBindVertexArray(LightVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

    Objshader.Use();
	Objshader.SetUniform1I("material.diffuse", 0);
	Objshader.SetUniform1I("material.specular", 1);

    auto textureID = loadTextrue("../../../Assets/textures/container2.png");
    while(!glfwWindowShouldClose(window)) {
        

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // don't forget to clear the stencil buffer!

		view = camera.GetViewMatrix();

		Objshader.Use();
		Objshader.SetMat4("ViewMat", view);
		glm::mat4 model = glm::mat4(1.0f);
		Objshader.SetMat4("ModelMat", model);
		Objshader.SetUniform3f("ViewPos", camera.GetCameraPosition());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		// glActiveTexture(GL_TEXTURE1);
		// glBindTexture(GL_TEXTURE_2D, texture2);

        processInput(window);
		glBindVertexArray(ObjVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(ObjVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		Lightshader.Use();
		Lightshader.SetMat4("ViewMat", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		Lightshader.SetMat4("ModelMat", model);
		glBindVertexArray(LightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}