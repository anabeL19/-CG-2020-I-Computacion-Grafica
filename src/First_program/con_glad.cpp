#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

double red_value=0.0, green_value=0.0, blue_value =0.0;
int i=0;
int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
    glfwSetKeyCallback(window, key_callback);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // render
        // ------
        glClearColor(red_value, green_value, blue_value, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // input
        // -----
        processInput(window);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (i==0 && key == GLFW_KEY_R && action == GLFW_PRESS) {
        red_value = 1.0;  i+=1;
    }
    else if (i==1 && key == GLFW_KEY_R && action == GLFW_PRESS) {
            red_value = 0.0; i=0;
    }

    else if (i==0 && key == GLFW_KEY_G && action == GLFW_PRESS) {
        green_value = 1.0;  i+=1;
    }
    else if (i==1 && key == GLFW_KEY_G && action == GLFW_PRESS) { 
        green_value = 0.0;  i=0;
    }

    else if (i==0 && key == GLFW_KEY_B && action == GLFW_PRESS) {
        blue_value = 1.0;   i+=1;
    }
    else if (i==1 && key == GLFW_KEY_B && action == GLFW_PRESS) {
        blue_value = 0.0;   i=0;
    }
    else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        red_value += 0.1;
        green_value += 0.1;
        blue_value += 0.1;
    }
    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        red_value -= 0.1;
        green_value -= 0.1;
        blue_value -= 0.1;
    }
}
