#include <iostream>
#include <string_view>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "util.h"
#include "Model.h"

void renderCube();
void renderQuad();


unsigned int gBuffer;
unsigned int gPosition, gNormal, gAlbedoSpec;
unsigned int depthTexture, finalTexture;
void drawQuad(Shader& shader, unsigned int textureID);

const unsigned int NR_LIGHTS = 320;
std::vector<glm::vec3> lightPositions;
std::vector<glm::vec3> lightColors;
std::vector<glm::vec3> objectPositions;
        const float constant = 1.0f; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
        const float linear = 0.7f;
        const float quadratic = 1.8f;

float calcPointLightSphere(const unsigned int id);

void startFrame();

void geometryPass(Shader& shader, Model& model);
void stencilPass(unsigned int id, Shader& shader, Model& model);
void pointLightPass(unsigned int id, Shader& shader, Model& model);
void finalPass();

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
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
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture moUse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failec to initialize GLAD\n";
        return -1;
    }

    // build and compile shaders
    // -------------------------
    Shader shaderGeometryPass("../../../Assets/shaders/8.1.g_buffer.glsl");
    Shader shaderLightingPass("../../../Assets/shaders/8.4.deferred_shading.glsl");
    Shader shader_debug("../../../Assets/shaders/debug_quad_frame.glsl");
    Shader sphere_shader("../../../Assets/shaders/8.4.sphere_shader.glsl");
    Shader null_shader("../../../Assets/shaders/8.4.null_shader.glsl");

    // load models
    // -----------
    Model backpack("../../../Assets/models/backpack/backpack.obj");
    Model light_sphere("../../../Assets/models/sphere.obj");

    objectPositions.push_back(glm::vec3(-3.0,  -0.5, -3.0));
    objectPositions.push_back(glm::vec3( 0.0,  -0.5, -3.0));
    objectPositions.push_back(glm::vec3( 3.0,  -0.5, -3.0));
    objectPositions.push_back(glm::vec3(-3.0,  -0.5,  0.0));
    objectPositions.push_back(glm::vec3( 0.0,  -0.5,  0.0));
    objectPositions.push_back(glm::vec3( 3.0,  -0.5,  0.0));
    objectPositions.push_back(glm::vec3(-3.0,  -0.5,  3.0));
    objectPositions.push_back(glm::vec3( 0.0,  -0.5,  3.0));
    objectPositions.push_back(glm::vec3( 3.0,  -0.5,  3.0));

#pragma region init framebuffer
    // configure g-buffer framebuffer
    // ------------------------------
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    // position color buffer
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
    // normal color buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
    // color + specular color buffer
    glGenTextures(1, &gAlbedoSpec);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
    // unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    // glDrawBuffers(3, attachments);

    // depth
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

    glGenTextures(1, &finalTexture);
    glBindTexture(GL_TEXTURE_2D, finalTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, finalTexture, 0);

    // finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
#pragma endregion

    // lighting info
    // -------------
    srand(13);
    for (unsigned int i = 0; i < NR_LIGHTS; i++)
    {
        // calculate slightly random offsets
        float xPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
        float yPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 4.0);
        float zPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
        lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
        // also calculate random color
        float rColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
        float gColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
        float bColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
        lightColors.push_back(glm::vec3(rColor, gColor, bColor));
    }

    // shader configuration
    // --------------------
    shaderLightingPass.Use();
    shaderLightingPass.SetInt("gPosition", 0);
    shaderLightingPass.SetInt("gNormal", 1);
    shaderLightingPass.SetInt("gAlbedoSpec", 2);

    shader_debug.Use();
    shader_debug.SetInt("screenTexture", 0);

    unsigned int choose = 0;
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        float cameraSpeed = 0.1f;
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
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
               camera.GetCameraPosition().y -= 1.0f * cameraSpeed;
        if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
            choose = gPosition;
        if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
            choose = gNormal;
        if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
            choose = gAlbedoSpec;
        if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
            choose = finalTexture;
        // render
        // ------
        // glClear(GL_COLOR_BUFFER_BIT);
        startFrame();
        geometryPass(shaderGeometryPass, backpack);

        glEnable(GL_STENCIL_TEST);
        for(unsigned int i = 0; i < NR_LIGHTS; ++i)
        {
            stencilPass(i, null_shader, light_sphere);
            pointLightPass(i, shaderLightingPass, light_sphere);
        }

        glDisable(GL_STENCIL_TEST);

        finalPass();
        // drawQuad(shader_debug, choose);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------

    glfwTerminate();
    return 0;
}
void startFrame()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT4);
    glClear(GL_COLOR_BUFFER_BIT);
}

void geometryPass(Shader& shader, Model& backpack)
{
    shader.Use();
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);
    GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0,
                                                     GL_COLOR_ATTACHMENT1,
                                                     GL_COLOR_ATTACHMENT2 };

    glDrawBuffers(3, DrawBuffers);
    
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    shader.SetMat4("projection", projection);
    shader.SetMat4("view", view);
    for (unsigned int i = 0; i < objectPositions.size(); i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, objectPositions[i]);
        model = glm::scale(model, glm::vec3(0.5f));
        shader.SetMat4("model", model);
        backpack.Draw(shader);
    }
    glDepthMask(GL_FALSE);
}

void stencilPass(unsigned int id, Shader& shader, Model& model)
{
    shader.Use();

    // disable the draw buffer
    glDrawBuffer(GL_NONE);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClear(GL_STENCIL_BUFFER_BIT);

    glStencilFunc(GL_ALWAYS, 0, 0);

    glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
    glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

    glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model_mat = glm::mat4(1.0f);

    shader.SetMat4("view", view);
    shader.SetMat4("projection", projection);
    float radius = calcPointLightSphere(id);
    model_mat = glm::scale(model_mat, glm::vec3(radius));
    model_mat = glm::translate(model_mat, lightPositions[id]);
    shader.SetMat4("model", model_mat);
    model.Draw(shader);
}

void pointLightPass(unsigned int id, Shader& shader, Model& model)
{
    glDrawBuffer(GL_COLOR_ATTACHMENT4);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec); 

    shader.Use();
    glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model_mat = glm::mat4(1.0f);

    shader.SetMat4("view", view);
    shader.SetMat4("projection", projection);
    float radius = calcPointLightSphere(id);
    model_mat = glm::scale(model_mat, glm::vec3(radius));
    model_mat = glm::translate(model_mat, lightPositions[id]);
    shader.SetMat4("model", model_mat);

    shader.SetVec3("viewPos", camera.GetCameraPosition());

    shader.SetVec3("light.Position", lightPositions[id]);
    shader.SetVec3("light.Color", lightColors[id]);
    shader.SetFloat("light.Linear", 0.7f);
    shader.SetFloat("light.Quadratic", 1.8f);
    glm::vec2 screen_size =  glm::vec2(SCR_WIDTH, SCR_HEIGHT);
    shader.SetVec2("gScreenSize", screen_size);

    glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

    // 需要混合
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    model.Draw(shader);    

    glCullFace(GL_BACK);
    glDisable(GL_BLEND);
}

void finalPass()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
    glReadBuffer(GL_COLOR_ATTACHMENT4);

    glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT,
                          0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

// The calculation solves a quadratic equation (see http://en.wikipedia.org/wiki/Quadratic_equation)
float calcPointLightSphere(const unsigned int id)
{
    // update attenuation parameters and calculate radius


        // then calculate radius of light volume/sphere
        const float maxBrightness = std::fmaxf(std::fmaxf(lightColors[id].r, lightColors[id].g), lightColors[id].b);
        float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 1.0f) * maxBrightness))) / (2.0f * quadratic);  

    return radius * 5.f;
}

void drawQuad(Shader& shader, unsigned int textureID)
{
        glDisable(GL_DEPTH_TEST);
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates. NOTE that this plane is now much smaller and at the top of the screen
        // positions   // texCoords
        -1.0f,   1.0f,  0.0f, 1.0f,
        -1.0f,  -1.0f,  0.0f, 0.0f,
         1.0f,  -1.0f,  1.0f, 0.0f,

        -1.0f,   1.0f,  0.0f, 1.0f,
         1.0f,  -1.0f,  1.0f, 0.0f,
         1.0f,   1.0f,  1.0f, 1.0f
    };
    // screen quad VAO
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    shader.Use();
    glBindVertexArray(quadVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);	// use the color attachment texture as the texture of the quad plane
    glDrawArrays(GL_TRIANGLES, 0, 6);
        glEnable(GL_DEPTH_TEST);
}

