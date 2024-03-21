#include <iostream>
#include <functional>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

inline float lastX = 0.0f;
inline float lastY = 0.0f;
inline bool firstMouse = true;

inline Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if(firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; 
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

static void processInput(GLFWwindow* window)
{
	float cameraSpeed = 0.001f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.GetCameraPosition() += cameraSpeed * camera.GetCameraFront();
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.GetCameraPosition() -= cameraSpeed * camera.GetCameraFront();
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.GetCameraPosition() -= glm::normalize(glm::cross(camera.GetCameraFront(), camera.GetCameraUp())) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.GetCameraPosition() += glm::normalize(glm::cross(camera.GetCameraFront(), camera.GetCameraUp())) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.GetCameraPosition().y += 1.0f * cameraSpeed;

}

static std::array<std::array<float, 4>, 4> getPerspectiveMatrix(float fov, float aspect, float zNear, float zFar)
{
	float tanHalfFovy = std::tan(fov/static_cast<float>(2));
	std::array<std::array<float, 4>, 4> ret{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, 0.0f};
	ret[0][0] = static_cast<float>(1) / (aspect * tanHalfFovy);
	ret[1][1] = static_cast<float>(1) / (tanHalfFovy);
	ret[2][2] = (zFar + zNear) / (zNear - zFar);
	ret[2][3] = -static_cast<float>(1);
	ret[3][2] = -(static_cast<float>(2) * zFar * zNear) / (zFar - zNear);
	return ret;
}

int main()
{
	std::cout << "hello opengl!" << std::endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "learnopengl", nullptr, nullptr);
    if(window == nullptr)
    {
        std::cout << "Fail to create opengl window!\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height)
	{glViewport(0, 0, width, height);});
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	// glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)
	{
		if(firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; 
		lastX = xpos;
		lastY = ypos;
		camera.ProcessMouseMovement(xoffset, yoffset);
	});

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
	glEnable(GL_DEPTH_TEST);
	// glDepthFunc(GL_ALWAYS);

	glViewport(0, 0, 800, 600);	

	// Shader Lightshader(std::string("../../../Assets/shaders/LightShader.glsl")), Objshader(std::string("../../../Assets/shaders/ObjectShader.glsl"));
	// Lightshader.Use();

	// glm::mat4 view = camera.GetViewMatrix();
    // Lightshader.SetMat4("ViewMat", view);
	// glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    // Lightshader.SetMat4("ProjectionMat", projection);

	// Objshader.Use();

	// glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
	// Objshader.SetUniform3f("objectColor", objectColor);
	// glm::vec3 lightPos(0.0f, 10.0f, 0.0f);
	// Objshader.SetUniform3f("lightPos", lightPos);

	// Objshader.SetMat4("ViewMat", view);
	// Objshader.SetUniform3f("ViewPos", camera.GetCameraPosition());
    // Objshader.SetMat4("ProjectionMat", projection);

	// // Objshader.SetVec3("material.ambient",  1.0f, 0.5f, 0.31f);
	// // Objshader.SetVec3("material.diffuse",  1.0f, 0.5f, 0.31f);
	// Objshader.SetVec3("material.specular", 0.5f, 0.5f, 0.5f);
	// Objshader.SetFloat("material.shininess", 32.0f);

	// Objshader.SetVec3("light.ambient",  0.2f, 0.2f, 0.2f);
	// Objshader.SetVec3("light.diffuse",  0.5f, 0.5f, 0.5f); // 将光照调暗了一些以搭配场景
	// Objshader.SetVec3("light.specular", 1.0f, 1.0f, 1.0f); 


    // float vertices[] = {
    // // positions          // normals           // texture coords
    // -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    //  0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
    //  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    //  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    // -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    // -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    // -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
    //  0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
    //  0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    //  0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    // -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    // -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    // -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    // -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    // -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    // -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    // -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    // -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    //  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    //  0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    //  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    //  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    //  0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    //  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    // -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
    //  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    //  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    //  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    // -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    // -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    // -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
    //  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    //  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    //  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    // -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    // -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    // };
    // // world space positions of our cubes


	// unsigned int indices[] = {  
    //     0, 1, 3, // first triangle
    //     1, 2, 3  // second triangle
    // };

	// unsigned int LightVAO, ObjVAO;
	// glGenVertexArrays(1, &LightVAO);
	// glBindVertexArray(LightVAO);

	// unsigned int VBO;
	// glGenBuffers(1, &VBO);
	// glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// // bind EBO after binding VBO
	// unsigned int EBO;
	// glGenBuffers(1, &EBO);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



	// glGenVertexArrays(1, &ObjVAO);
	// glBindVertexArray(ObjVAO);
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	// glEnableVertexAttribArray(0);
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	// glEnableVertexAttribArray(1);
	// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// glEnableVertexAttribArray(2);
	
	// glBindVertexArray(LightVAO);
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	// glEnableVertexAttribArray(0);

	// /* texture */
	// unsigned int texture1, texture2;
	// glGenTextures(1, &texture1);
	// // glActiveTexture(GL_TEXTURE0);
	// glBindTexture(GL_TEXTURE_2D, texture1);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// stbi_set_flip_vertically_on_load(true);
	// int width, height, nrChannels;
	// unsigned char *data = stbi_load("../../../Assets/textures/container2.png", &width, &height, &nrChannels, 0);
	// if (data)
	// {
	// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	// 	glGenerateMipmap(GL_TEXTURE_2D);
	// 	std::cout << "load texture successfully" << std::endl;
	// }
	// else
	// {
	// 	std::cout << "Failed to load texture" << std::endl;
	// }
	// stbi_image_free(data);

	// glGenTextures(1, &texture2);
	// glBindTexture(GL_TEXTURE_2D, texture2);

	// data = stbi_load("../../../Assets/textures/container2_specular.png", &width, &height, &nrChannels, 0);
	// if (data)
	// {
	// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	// 	glGenerateMipmap(GL_TEXTURE_2D);
	// 	std::cout << "load texture successfully" << std::endl;
	// }
	// else
	// {
	// 	std::cout << "Failed to load texture" << std::endl;
	// }
	// stbi_image_free(data);

	// Objshader.Use();
	// Objshader.SetUniform1I("material.diffuse", 0);
	// Objshader.SetUniform1I("material.specular", 1);
#pragma region sixiwanzi

	Shader ourShader(std::string("../../../Assets/shaders/modelShader.glsl"));
	// Model ourModel(std::string("../../../Assets/models/nanosuit/nanosuit.obj"));
	Model LianModel(std::string("../../../Assets/models/lian/Lian.pmx"));
	// Model TiandouModel(std::string("../../../Assets/models/tiandou/Tiandou.pmx"));
	Model MumuModel(std::string("../../../Assets/models/mumu/Mumu.pmx"));
	// Model YouyiModel(std::string("../../../Assets/models/youyi/Youyi.pmx"));

#pragma endregion
	double initT = glfwGetTime();


	while(!glfwWindowShouldClose(window))  
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// input
		processInput(window);

		// don't forget to enable shader before setting uniforms

#pragma region sixiwanzi
        ourShader.Use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        // auto projection = getPerspectiveMatrix(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.SetMat4("projection", projection);
        ourShader.SetMat4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
        ourShader.SetMat4("model", model);
        LianModel.Draw(ourShader);

		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));
        ourShader.SetMat4("model", model);
        MumuModel.Draw(ourShader);

		// model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        // ourShader.SetMat4("model", model);
        // TiandouModel.Draw(ourShader);

        // ourShader.SetMat4("model", model);
        // YouyiModel.Draw(ourShader);

#pragma endregion

		// Lightshader.Use();

		// view = camera.GetViewMatrix();

		// Objshader.Use();
		// Objshader.SetMat4("ViewMat", view);
		// glm::mat4 model = glm::mat4(1.0f);

		
		// Objshader.SetMat4("ModelMat", model);
		// Objshader.SetUniform3f("ViewPos", camera.GetCameraPosition());


		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, texture1);
		// glActiveTexture(GL_TEXTURE1);
		// glBindTexture(GL_TEXTURE_2D, texture2);

		// // glBindVertexArray(ObjVAO);

		// glDrawArrays(GL_TRIANGLES, 0, 36);

		// glBindVertexArray(LightVAO);
		// Lightshader.Use();
		// Lightshader.SetMat4("ViewMat", view);
		// model = glm::mat4(1.0f);
		// model = glm::translate(model, lightPos);
		// model = glm::scale(model, glm::vec3(0.2f));
		// Lightshader.SetMat4("ModelMat", model);
		// glDrawArrays(GL_TRIANGLES, 0, 36);

	    glfwSwapBuffers(window);
	    glfwPollEvents();      
	}	

	// glDeleteVertexArrays(1, &LightVAO);
	// glDeleteVertexArrays(1, &ObjVAO);
    // glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}