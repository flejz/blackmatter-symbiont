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

#define NR_LIGHT_ROW 39
#define NR_LIGHT_MID NR_LIGHT_ROW / 2
#define NR_LIGHTS NR_LIGHT_ROW * NR_LIGHT_ROW
#define NR_SPHERE_ROW 13
#define NR_SPHERE_MID NR_SPHERE_ROW / 2
#define NR_SPHERES NR_SPHERE_ROW * NR_SPHERE_ROW
#define SCR_WIDTH 1920
#define SCR_HEIGHT 1080

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// camera
Camera camera(glm::vec3(0.f, NR_LIGHT_MID, NR_SPHERE_MID + 10));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

string genLightPositionString(int i, std::string prop) {
    string number = to_string(i);
    return ("lights[" + number + "]." + prop).c_str();
}

// light positions
glm::vec3* genLightPositions() {
    static glm::vec3 pos[NR_LIGHTS];
    for (int i = 0; i < NR_LIGHT_ROW; i++) 
        for (int j = 0; j < NR_LIGHT_ROW; j++)
            pos[(i * NR_LIGHT_ROW) + j] = glm::vec3((float)i - NR_LIGHT_MID, (float)j, -20.0f);

    return pos;
}

glm::vec3* lightPositions = genLightPositions();

// sphere positions
glm::vec3* genSphereInitPositions() {
    static glm::vec3 pos[NR_SPHERES];
    for (int i = 0; i < NR_SPHERE_ROW; i++) 
        for (int j = 0; j < NR_SPHERE_ROW; j++)
            pos[(i * NR_SPHERE_ROW) + j] = glm::vec3((float)i - NR_SPHERE_MID, NR_LIGHT_MID, (float)j - NR_SPHERE_MID);

    return pos;
}

glm::vec3* spherePositions = genSphereInitPositions();

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Dark Matter", NULL, NULL);
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
        std::cout << "Failed to initialize GLEW" << std::endl;
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
    

    // check the max quantity of uniform vectors available
    int maxUniformVectors;
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &maxUniformVectors);

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
        glClearColor(0.02f, 0.02f, 0.02f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // variables
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model;

        // don't forget to enable shader before setting uniforms
        sphereShader.use();
        sphereShader.setVec3("viewPos", camera.Position);

        float red = glm::abs(sin(glfwGetTime()));
        red = red > 0.8f ? 0.8f : red < 0.15f ? 0.15f : red;

        // ****** LIGHT
        glm::vec3 lightColor(red, 0.0f, 0.0f);
        glm::vec3 diffuseColor = lightColor   * glm::vec3(0.15f); // decrease the influence
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.08f); // low influence

        for (int i = 0; i < NR_LIGHTS; i++)
        {
            glm::vec3 lightPos = *(lightPositions + i);

            sphereShader.setVec3(genLightPositionString(i, "position"), lightPos);
            sphereShader.setVec3(genLightPositionString(i, "ambient"), ambientColor);
            sphereShader.setVec3(genLightPositionString(i, "diffuse"), diffuseColor);
            sphereShader.setVec3(genLightPositionString(i, "specular"), lightColor);

            sphereShader.setFloat(genLightPositionString(i, "constant"), 1.0f);
            sphereShader.setFloat(genLightPositionString(i, "linear"), 0.09f);
            sphereShader.setFloat(genLightPositionString(i, "quadratic"), 0.032f);
        }

        // material properties
        sphereShader.setVec3("material.ambient", 0.7f, 0.7f, 0.7f);
        sphereShader.setVec3("material.diffuse", 0.9f, 0.9f, 0.9f);
        sphereShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular our doesn't have full effect on this object's material
        sphereShader.setFloat("material.shininess", 32.0f);

        // view/projection transformations
        sphereShader.setMat4("projection", projection);
        sphereShader.setMat4("view", view);

        float rotationAngle = glm::radians(45.0f);

        // render spheres
        float sphereScaleFactor = 0.4f;
        for (int i = 0; i < NR_SPHERES; i++) 
        {
            glm::vec3 spherePos = *(spherePositions + i);
            spherePos.y += sin(glfwGetTime() + i); // diagonal
            // spherePos.y += sin(glfwGetTime() + i / NR_SPHERE_ROW); // horizonal

            model = glm::mat4(1.0f);
            model = glm::rotate(model, rotationAngle, glm::vec3(0, 1, 0));
            model = glm::translate(model, spherePos); // translate it down so it's at the center of the scene
            model = glm::scale(model, glm::vec3(sphereScaleFactor));	// it's a bit too big for our scene, so scale it down
            sphereShader.setMat4("model", model);
            sphereModel.Draw(sphereShader);
        }

        // draw the light sources
        float lightScaleFactor = 0.045f;
        for (int i = 0; i < NR_LIGHTS; i++)
        {
            glm::vec3 lightPos = *(lightPositions + i);

            lightShader.use();
            lightShader.setVec3("lightColor", lightColor);
            lightShader.setMat4("projection", projection);
            lightShader.setMat4("view", view);
            model = glm::mat4(1.0f);
            model = glm::translate(model, lightPos);
            model = glm::scale(model, glm::vec3(lightScaleFactor));
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
