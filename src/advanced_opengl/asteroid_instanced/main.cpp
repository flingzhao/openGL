#include <iostream>
#include <string_view>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "util.h"
Camera mycamera(glm::vec3(0.0f, 0.0f, 55.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

void my_mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

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

    mycamera.ProcessMouseMovement(xoffset, yoffset);
}

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
    glfwSetCursorPosCallback(window, my_mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    Shader planet_shader("../../../Assets/shaders/planet.glsl");
    Shader asteroid_shader("../../../Assets/shaders/asteroid_instanced.glsl");

    // load model
    // ----------
    Model rock("../../../Assets/models/rock/rock.obj");
    Model planet("../../../Assets/models/planet/planet.obj");

    unsigned int amount = 100000;
    glm::mat4* modelMatrices;
    modelMatrices = new glm::mat4[amount];
    srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed
    float radius = 150.0;
    float offset = 25.0f;
    for (unsigned int i = 0; i < amount; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. scale: Scale between 0.05 and 0.25f
        float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
        model = glm::scale(model, glm::vec3(scale));

        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = static_cast<float>((rand() % 360));
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        modelMatrices[i] = model;
    }    

    // 顶点缓冲对象
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    for(unsigned int i = 0; i < rock.getMesh().size(); i++)
    {
        unsigned int VAO = rock.getMesh()[i].getVAO();
        glBindVertexArray(VAO);
        // 顶点属性
        GLsizei vec4Size = sizeof(glm::vec4);
        glEnableVertexAttribArray(3); 
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
        glEnableVertexAttribArray(4); 
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
        glEnableVertexAttribArray(5); 
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
        glEnableVertexAttribArray(6); 
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }  

    auto processInputLambda = [](GLFWwindow *window)
    {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        
        float cameraSpeed = 1.f;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            mycamera.GetCameraPosition() += cameraSpeed * mycamera.GetCameraFront();
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            mycamera.GetCameraPosition() -= cameraSpeed * mycamera.GetCameraFront();
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            mycamera.GetCameraPosition() -= glm::normalize(glm::cross(mycamera.GetCameraFront(), mycamera.GetCameraUp())) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            mycamera.GetCameraPosition() += glm::normalize(glm::cross(mycamera.GetCameraFront(), mycamera.GetCameraUp())) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            mycamera.GetCameraPosition().y += 1.0f * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            mycamera.GetCameraPosition().y -= 1.0f * cameraSpeed;
    };

    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInputLambda(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // don't forget to clear the stencil buffer!

        // configure transformation matrices
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = mycamera.GetViewMatrix();;
        planet_shader.Use();
        planet_shader.SetMat4("projection", projection);
        planet_shader.SetMat4("view", view);

        // draw planet
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        planet_shader.SetMat4("model", model);
        planet.Draw(planet_shader);


        // draw meteorites
        asteroid_shader.Use();
        asteroid_shader.SetMat4("projection", projection);
        asteroid_shader.SetMat4("view", view);
        asteroid_shader.SetInt("texture_diffuse1", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, rock.getTextureLoaded()[0].id); // note: we also made the textures_loaded vector public (instead of private) from the model class.
        for (unsigned int i = 0; i < rock.getMesh().size(); i++)
        {
            glBindVertexArray(rock.getMesh()[i].getVAO());
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(rock.getMesh()[i].m_Indices.size()), GL_UNSIGNED_INT, 0, amount);
            glBindVertexArray(0);
        }


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------

    glfwTerminate();
    return 0;
}