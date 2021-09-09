#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/filesystem.h"
#include "include/shader_m.h"
#include "include/camera.h"
#include "include/model.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

string lightGen(int i, std::string prop) {
    string number = to_string(i);
    return ("lights[" + number + "]." + prop).c_str();
}

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPositions[] = {
    glm::vec3(0.0f, -1.0f, 3.0f),
    glm::vec3(1.0f, -1.0f, 3.0f),
    glm::vec3(2.0f, -1.0f, 3.0f),
    glm::vec3(3.0f, -1.0f, 3.0f),
    glm::vec3(4.0f, -1.0f, 3.0f),
    glm::vec3(5.0f, -1.0f, 3.0f),
    glm::vec3(6.0f, -1.0f, 3.0f),
    glm::vec3(7.0f, -1.0f, 3.0f),
    glm::vec3(0.0f, 0.0f, 3.0f),
    glm::vec3(1.0f, 0.0f, 3.0f),
    glm::vec3(2.0f, 0.0f, 3.0f),
    glm::vec3(3.0f, 0.0f, 3.0f),
    glm::vec3(4.0f, 0.0f, 3.0f),
    glm::vec3(5.0f, 0.0f, 3.0f),
    glm::vec3(6.0f, 0.0f, 3.0f),
    glm::vec3(7.0f, 0.0f, 3.0f),
    glm::vec3(0.0f, 1.0f, 3.0f),
    glm::vec3(1.0f, 1.0f, 3.0f),
    glm::vec3(2.0f, 1.0f, 3.0f),
    glm::vec3(3.0f, 1.0f, 3.0f),
    glm::vec3(4.0f, 1.0f, 3.0f),
    glm::vec3(5.0f, 1.0f, 3.0f),
    glm::vec3(6.0f, 1.0f, 3.0f),
    glm::vec3(7.0f, 1.0f, 3.0f),
};
int lightPositionsCount = sizeof(lightPositions) / sizeof(glm::vec3);

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learning OpenGL", NULL, NULL);
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
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader lightShader("light_cube.vs", "light_cube.fs");
    Shader sphereShader("model_material_light.vs", "model_material_light.fs");

    // load models
    // -----------
    Model lightModel(FileSystem::getPath("resources/objects/sphere/scene.gltf"));
    Model sphereModel(FileSystem::getPath("resources/objects/scifi_hexsphere/scene.gltf"));

    
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
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // variables
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model;

        // don't forget to enable shader before setting uniforms
        sphereShader.use();
        sphereShader.setVec3("viewPos", camera.Position);

        // ****** LIGHT
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        glm::vec3 diffuseColor = lightColor   * glm::vec3(0.3f); // decrease the influence
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.08f); // low influence

        for (int i = 0; i < lightPositionsCount; i++)
        {
            string number = to_string(i);
            glm::vec3 lightPos = lightPositions[i];
            
            sphereShader.setVec3(lightGen(i, "position"), lightPos);
            sphereShader.setVec3(lightGen(i, "ambient"), ambientColor);
            sphereShader.setVec3(lightGen(i, "diffuse"), diffuseColor);
            sphereShader.setVec3(lightGen(i, "specular"), 1.0f, 1.0f, 1.0f);

            sphereShader.setFloat(lightGen(i, "constant"), 1.0f);
            sphereShader.setFloat(lightGen(i, "linear"), 0.09f);
            sphereShader.setFloat(lightGen(i, "quadratic"), 0.032f);
        }
        


        // material properties
        sphereShader.setVec3("material.ambient", 0.9f, 0.9f, 0.9f);
        sphereShader.setVec3("material.diffuse", 0.9f, 0.9f, 0.9f);
        sphereShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular our doesn't have full effect on this object's material
        sphereShader.setFloat("material.shininess", 32.0f);

        // view/projection transformations
        sphereShader.setMat4("projection", projection);
        sphereShader.setMat4("view", view);

        // render the loaded model
        float scaleFactor = 1.f;
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f)); // translate it down so it's at the center of the scene
        // model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
        model = glm::scale(model, scaleFactor * glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        sphereShader.setMat4("model", model);
        sphereModel.Draw(sphereShader);

        for (int i = 0; i < lightPositionsCount; i++)
        {
            glm::vec3 lightPos = lightPositions[i];

            // also draw the lamp object
            lightShader.use();
            lightShader.setMat4("projection", projection);
            lightShader.setMat4("view", view);
            model = glm::mat4(1.0f);
            model = glm::translate(model, lightPos);
            model = glm::scale(model, glm::vec3(0.02f)); // a smaller cube
            lightShader.setMat4("model", model);
            lightModel.Draw(lightShader);
        }



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
void processInput(GLFWwindow *window)
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
