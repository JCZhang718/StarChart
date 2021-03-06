#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "model.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 2560;
const unsigned int SCR_HEIGHT = 1440;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 20.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float totalTime = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    //stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader("VertexShader.vs", "FragmentShader.fs");
    Shader ambientShader("VertexShader.vs", "AmbientFragmentShader.fs");

    // load models
    // -----------
    Model sunModel("Models/Sun/moon.obj");
    Model earthModel("Models/Earth/Earth 2K.obj");
    Model moonModel("Models/Moon/Moon 2k.obj");
    Model marsModel("Models/Mars/Mars 2k.obj");
    Model ufoModel("Models/UFO/UFO_Empty.obj");
    Model spaceshipModel("Models/Spaceship/Intergalactic_Spaceship-(Wavefront).obj");
    Model jetModel("Models/Jet/Futuristic combat jet.obj");
    Model rocketModel("Models/Rocket/rocket.obj");
    Model skyboxModel("Models/Skybox/skybox.obj");


    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        totalTime += deltaTime;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        ambientShader.use();
        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ambientShader.setMat4("projection", projection);
        ambientShader.setMat4("view", view);

        // render the sun model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::rotate(model, glm::radians(totalTime * 5.0f), glm::vec3(0.0f, 0.1f, 0.0f)); // Spin
        model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
        ambientShader.setMat4("model", model);
        sunModel.Draw(ambientShader);

        // render the skybox model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1000.0f, 1000.0f, 1000.0f));	// it's a bit too big for our scene, so scale it down
        ambientShader.setMat4("model", model);
        skyboxModel.Draw(ambientShader);

        // don't forget to enable shader before setting uniforms
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        ourShader.setVec3("viewPos", camera.Position);

        // render the earth model
        model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(totalTime * 5.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Spin around sun
        model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f)); // translate it to the right of sun
        model = glm::rotate(model, -glm::radians(totalTime * 25.0f), glm::vec3(0.0f, 0.1f, 0.0f)); // Spin
        model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        earthModel.Draw(ourShader);

        // render the moon model
        model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(totalTime * 5.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Spin around sun
        model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f)); // translate it to the right of sun
        model = glm::rotate(model, glm::radians(totalTime * 10.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Spin around earth
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f)); // translate it to the right of earth
        model = glm::rotate(model, -glm::radians(totalTime * 50.0f), glm::vec3(0.0f, 0.1f, 0.0f)); // Spin
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        moonModel.Draw(ourShader);

        // render the rocket model
        model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(totalTime * 5.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Spin around sun
        model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f)); // translate it to the right of sun
        model = glm::rotate(model, glm::radians(totalTime * 10.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Spin around earth
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f)); // translate it to the right of earth
        model = glm::rotate(model, glm::radians(totalTime * 50.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Spin around moon
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f)); // translate it above the moon
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Spin around moon
        model = glm::scale(model, glm::vec3(0.001f, 0.001f, 0.001f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        rocketModel.Draw(ourShader);

        // render the mars model
        model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(totalTime * 5.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Spin around sun
        model = glm::translate(model, glm::vec3(-15.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::rotate(model, -glm::radians(totalTime * 25.0f), glm::vec3(0.0f, 0.1f, 0.0f)); // Spin
        model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        marsModel.Draw(ourShader);

        // render the UFO model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(totalTime, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::translate(model, glm::vec3(-60.0f, 5.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::rotate(model, glm::radians(totalTime * 69.0f), glm::vec3(0.0f, 0.1f, 0.0f)); // Spin
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        ufoModel.Draw(ourShader);

        // render the spaceship model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0, 0.0f, totalTime)); // translate it down so it's at the center of the scene
        model = glm::translate(model, glm::vec3(0.0f, -5.0f, -60.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        spaceshipModel.Draw(ourShader);

        // render the jet model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(7.0f, 7.0f, -7.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        jetModel.Draw(ourShader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
