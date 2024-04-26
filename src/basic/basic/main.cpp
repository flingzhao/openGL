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
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glViewport(0, 0, 800, 600);	
#pragma region sixiwanzi

	Shader ourShader(std::string("../../../Assets/shaders/modelShader.glsl"));
	Shader shaderSingleColor(std::string("../../../Assets/shaders/stencil_testing_single_color.glsl"));
	// Model ourModel(std::string("../../../Assets/models/nanosuit/nanosuit.obj"));
	Model LianModel(std::string("../../../Assets/models/lian/Lian.pmx"));
	// Model TiandouModel(std::string("../../../Assets/models/tiandou/Tiandou.pmx"));
	Model MumuModel(std::string("../../../Assets/models/mumu/Mumu.pmx"));
	// Model YouyiModel(std::string("../../../Assets/models/youyi/Youyi.pmx"));

    Model nano(std::string("../../../Assets/models/nanosuit_reflection/nanosuit.obj"));
    Model light_sphere("../../../Assets/models/sphere.obj");
    Shader null_shader("../../../Assets/shaders/8.4.null_shader.glsl");
#pragma endregion
	double initT = glfwGetTime();


	while(!glfwWindowShouldClose(window))  
	{
		// input
		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms

        ourShader.Use();

        // // view/projection transformations
        // glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        // // auto projection = getPerspectiveMatrix(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        // glm::mat4 view = camera.GetViewMatrix();
        // ourShader.SetMat4("projection", projection);
        // ourShader.SetMat4("view", view);
        // // render the loaded model
        // glm::mat4 model = glm::mat4(1.0f);
        // model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        // model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
        // ourShader.SetMat4("model", model);
		// nano.Draw(ourShader);

		null_shader.Use();
		glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)800 / (float)600, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model_mat = glm::mat4(1.0f);

        null_shader.SetMat4("view", view);
        null_shader.SetMat4("projection", projection);
        null_shader.SetMat4("model", model_mat);
        light_sphere.Draw(null_shader);
	
#pragma region sixiwanzi

		
		// // glstencilfunc(gl_always, 1, 0xff);
		// // glstencilmask(0xff);

        // // lianmodel.draw(ourshader);

		// // model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));
        // // ourshader.setmat4("model", model);
        // // mumumodel.draw(ourshader);

		// // model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        // // ourShader.SetMat4("model", model);
        // // TiandouModel.Draw(ourShader);

        // // ourShader.SetMat4("model", model);
        // // YouyiModel.Draw(ourShader);

		// // draw outline
		// shaderSingleColor.Use();
		// model = glm::mat4(1.0f);
		// model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		// model = glm::scale(model, glm::vec3(0.103f, 0.103f, 0.103f));
		// shaderSingleColor.SetMat4("model", model);
		// shaderSingleColor.SetMat4("projection", projection);
		// shaderSingleColor.SetMat4("view", view);
		// glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		// glStencilMask(0x00);
		// glDisable(GL_DEPTH_TEST);

		// // shaderSingleColor.Use();
		// // LianModel.Draw(shaderSingleColor);
		// LianModel.Draw(shaderSingleColor);

		// glBindVertexArray(0);
		// glStencilMask(0xFF);
		// glStencilFunc(GL_ALWAYS, 0, 0xFF);
		// glEnable(GL_DEPTH_TEST);

#pragma endregion



	    glfwSwapBuffers(window);
	    glfwPollEvents();      
	}	


    glfwTerminate();
    return 0;
}