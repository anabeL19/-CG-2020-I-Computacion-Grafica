/* EJERCICIO INICIAL 
Visualización y movimiento de un triángulo
*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "lib/matrix.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow *window);

void menu();
void shape_vertex(float full_vertex[], int it, float *shape_vtx);
void render_matrix(Matrix &Mmain, Op &op);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//Transformations
const int cant_elem = 9;
float var_scale = 0.15;
float var_tras = 0.15;
double angle = 9;

//Vector
const int r_v = 4;
const int c_v = 1;

float firstTriangle[] = {
    -0.9f, -0.5f, 0.0f,  // left 
    -0.0f, -0.5f, 0.0f,  // right
    -0.45f, 0.5f, 0.0f,  // top 
};

float secondTriangle[] = {
    0.0f, -0.5f, 0.0f,  // left
    0.9f, -0.5f, 0.0f,  // right
    0.45f, 0.5f, 0.0f   // top 
};

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);\n"
    "}\n\0";
const char *fragmentShader2Source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.5f, 0.5f, 0.0f, 1.0f);\n"
    "}\n\0";


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    // we skipped compile log checks this time for readability (if you do encounter issues, add the compile-checks! see previous code samples)
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER); // the first fragment shader that outputs the color orange
    unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER); // the second fragment shader that outputs the color orange
    
    unsigned int shaderProgram1 = glCreateProgram();
    unsigned int shaderProgramYellow = glCreateProgram(); 
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShader1, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader1);
    
    glShaderSource(fragmentShaderYellow, 1, &fragmentShader2Source, NULL);
    glCompileShader(fragmentShader1);

    // link the first program object
    glAttachShader(shaderProgram1, vertexShader);
    glAttachShader(shaderProgram1, fragmentShader1);
    glLinkProgram(shaderProgram1);
    
    //link object yellow
    glAttachShader(shaderProgramYellow, vertexShader);
    glAttachShader(shaderProgramYellow, fragmentShaderYellow);
    glLinkProgram(shaderProgramYellow);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs); // we can also generate multiple VAOs or buffers at the same time
    glGenBuffers(2, VBOs);

    // first triangle setup
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Vertex attributes stay the same
    glEnableVertexAttribArray(0);
    
    // second triangle setup
    glBindVertexArray(VAOs[1]);	// note that we bind to a different VAO now
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);	// and a different VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // because the vertex data is tightly packed we can also specify 0 as the vertex attribute's stride to let OpenGL figure it out
    glEnableVertexAttribArray(0);
    
    menu();
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram1);
        glBindVertexArray(VAOs[0]);
        // glDrawArrays(GL_TRIANGLES, 0, 3);	// this call should output an orange triangle
        
        glUseProgram(shaderProgramYellow);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);	// this call should output a yellow triangle

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shaderProgram1);
    glDeleteProgram(shaderProgramYellow);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

//-----------------------------      SHOW OPERATORS  --------------------
//-----------------------------------------------------------------------
void menu() {
    std::cout<<"                   MENU                     \n";
    std::cout<<"    KEY 1: Escala para incrementar tamaño \n";
    std::cout<<"    KEY 2: Escala para disminuir tamaño \n";
    std::cout<<"    KEY X: Rotacion en el eje x \n";
    std::cout<<"    KEY Y: Rotacion en el eje y \n";
    std::cout<<"    KEY Z: Rotación en el eje z \n";
    std::cout<<"    KEY A: Traslación a la izquierda \n";
    std::cout<<"    KEY D: Traslación a la derecha \n";
    std::cout<<"    KEY W: Traslación hacia arriba \n";
    std::cout<<"    KEY S: Traslación hacia abajo \n";
}

void shape_vertex(float full_vertex[cant_elem], int it, float *shape_vtx)
{
    for (int i=0; i<=3; i++) {
        shape_vtx[i] = full_vertex[it];  
        it++; 
    }
    shape_vtx[r_v-1] = 1.0f;
}

void render_matrix(Matrix &Mmain, Op &op)
{
    // std::cout<<"render matrix \n";
    // for c/vertex
    for (int i = 0; i < cant_elem; i+=3) {
        float shape_vtx[r_v] = {0,0,0,1.0};
        shape_vertex(firstTriangle, i, shape_vtx);
        Vector svtx(shape_vtx, r_v, c_v);

        Matrix r(Mmain.r, svtx.c);
        op.MultMatrixVector(svtx, r);
        firstTriangle[i] = r.M[0][0];
        firstTriangle[i+1] = r.M[1][0];
        firstTriangle[i+2] = r.M[2][0];

        //SECOND TRIANGLE
        // float shape_vtx2[r_v] = {0,0,0,1.0};
        // shape_vertex(secondTriangle, i, shape_vtx2);
        // Vector svtx2(shape_vtx, r_v, c_v);

        // Matrix r2(Mmain.r, svtx2.c);
        // op.MultMatrixVector(svtx2, r);
        // secondTriangle[i] = r2.M[0][0];
        // secondTriangle[i+1] = r2.M[1][0];
        // secondTriangle[i+2] = r2.M[2][0];
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
}


//----------------------------------       KEYS         -----------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if( key == GLFW_KEY_1 && action == GLFW_PRESS || key == GLFW_KEY_1 && action == GLFW_REPEAT) {
        Matrix Mmain(4,4);
        Mmain.Identity();

        float ini[r_v] = {1.0f + var_scale, 1.0f, 1.0f, 1.0f};
        Vector v(ini, 4,1);

        Op op(&Mmain);
        op.Scale(v);
        render_matrix(Mmain, op);
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS || key == GLFW_KEY_2 && action == GLFW_REPEAT)
    {
        Matrix Mmain(4,4);
        Mmain.Identity();

        float ini[r_v] = {1.0f - var_scale, 1.0f, 1.0f, 1.0f};
        Vector v(ini, 4,1);

        Op op(&Mmain);
        op.Scale(v);
        render_matrix(Mmain, op);
    }

    //ROTATION
    if (key == GLFW_KEY_Z && action == GLFW_PRESS || key == GLFW_KEY_Z && action == GLFW_REPEAT)
    {
        Matrix Mmain(4,4);
        Mmain.Identity();

        Op op(&Mmain);
        op.Rotation_z(angle);
        render_matrix(Mmain, op);
    }

    if (key == GLFW_KEY_Y && action == GLFW_PRESS || key == GLFW_KEY_Y && action == GLFW_REPEAT)
    {
        Matrix Mmain(4,4);
        Mmain.Identity();

        Op op(&Mmain);
        op.Rotation_y(angle);
        render_matrix(Mmain, op);
    }

    if (key == GLFW_KEY_X && action == GLFW_PRESS || key == GLFW_KEY_X && action == GLFW_REPEAT)
    {
        Matrix Mmain(4,4);
        Mmain.Identity();

        Op op(&Mmain);
        op.Rotation_x(angle);
        render_matrix(Mmain, op);
    }

    //TRASLATION
    if (key == GLFW_KEY_A && action == GLFW_PRESS || key == GLFW_KEY_A && action == GLFW_REPEAT)
    {
        Matrix Mmain(4,4);
        Mmain.Identity();

        float ini[r_v] = {-var_tras, 0.0f, 0.0f, 0.0f};
        Vector v(ini, 4,1);

        Op op(&Mmain);
        op.Traslation(v);
        render_matrix(Mmain, op);
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS || key == GLFW_KEY_D && action == GLFW_REPEAT)
    {
        Matrix Mmain(4,4);
        Mmain.Identity();

        float ini[r_v] = {var_tras, 0.0f, 0.0f, 0.0f};
        Vector v(ini, 4,1);

        Op op(&Mmain);
        op.Traslation(v);
        render_matrix(Mmain, op);
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS || key == GLFW_KEY_S && action == GLFW_REPEAT)
    {
        Matrix Mmain(4,4);
        Mmain.Identity();

        float ini[r_v] = {0.0f, -var_tras, 0.0f, 0.0f};
        Vector v(ini, 4,1);

        Op op(&Mmain);
        op.Traslation(v);
        render_matrix(Mmain, op); 
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS || key == GLFW_KEY_W && action == GLFW_REPEAT)
    {
        Matrix Mmain(4,4);
        Mmain.Identity();

        float ini[r_v] = {0.0f, var_tras, 0.0f, 0.0f};
        Vector v(ini, 4,1);

        Op op(&Mmain);
        op.Traslation(v);
        render_matrix(Mmain, op);   
    }
}