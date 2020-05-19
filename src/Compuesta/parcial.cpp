/* TRANSFORMACION COMPUESTA*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "matrix.h"
#include <iostream>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow *window);

void menu();
void Tshape_vertex(float full_vertex[], int it, float *shape_vtx);
void Sshape_vertex(float full_vertex[], int it, float *shape_vtx);
void Rshape_vertex(float full_vertex[], int it, float *shape_vtx);
void render_matrix(Matrix &Mmain, Op &op);

void animation_basic ();
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//Transformations
const int cant_elem0 = 9;
const int cant_elem1 = 12;
const int cant_elem2 = 12;
float var_scale = 0.10;
float var_tras = 0.40;
double angle = 45;

//Vector
const int r_v = 4;
const int c_v = 1;

//
int i=0;
std::string shape_t= "t";
std::string shape_s= "s";
std::string shape_r= "r";
float red_value , green_value, blue_value;
float red_value1 , green_value1, blue_value1;
float red_value2 , green_value2, blue_value2;
float valcolor=0.8;

//Animations
bool a_basic = false;
float inc = 0.04;
int t_random = 0;
std::string option_traslation = " ";
std::string traslation_right = "tright";
std::string traslation_left = "tleft";
std::string traslation_up = "tup";
std::string traslation_down = "tdown";
float t=0.0;
unsigned int VBOs[3], VAOs[3], EBOs[2];
float vertexsTriangle[] = {
    //triangulo
    0.0f, 0.2f, 0.0f,  // top 
    -0.1f, -0.1f, 0.0f,  // bottom right
    0.1f, -0.1f, 0.0f,  // bottom left
};
float vertexsSquare[] = {
    //cuadrado
    0.2f,  0.2f, 0.0f,  // top right3
    0.2f, -0.1f, 0.0f,  // bottom right4
    0.4f, -0.1f, 0.0f,  // bottom left5
    0.4f,  0.2f, 0.0f,   // top left 6
};
float vertexsRhombus[] = {
    //rombo
    0.6f+t, -0.6f+t, 0.0f+t,  // top 7
    0.6f+t, -0.9f+t, 0.0f+t,  // bottom 8 
    0.75f+t, -0.74f+t, 0.0f+t,  // right 9
    0.45f+t, -0.74f+t, 0.0f+t   // left 10
};

unsigned int indicesSquare[] = {
    0, 1, 3, //top right - bottom right - top left 
    1, 2, 3 //bottom right - bottom left - top left 
};
unsigned int indicesRhombus[] = {
    //rombo
    2, 0, 3,
    2, 1, 3
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
    "   FragColor = vec4(0.5f, 0.3f, 0.5f, 1.0f);\n"
    "}\n\0";
const char *fragmentShader1Source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";
const char *fragmentShader2Source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 ourColorR;\n"
    "void main()\n"
    "{\n"
    "   FragColor = ourColorR;\n"
    "}\n\0";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "TRANSFORMACION COMPUESTA", NULL, NULL);
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
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); 
    unsigned int fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER); 

    unsigned int shaderProgram = glCreateProgram();
    unsigned int shaderProgram1 = glCreateProgram();
    unsigned int shaderProgram2 = glCreateProgram();
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glShaderSource(fragmentShader1, 1, &fragmentShader1Source, NULL);
    glCompileShader(fragmentShader1);
    glShaderSource(fragmentShader2, 1, &fragmentShader2Source, NULL);
    glCompileShader(fragmentShader2);

    // link the program object
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // link the program object 2
    glAttachShader(shaderProgram1, vertexShader);
    glAttachShader(shaderProgram1, fragmentShader1);
    glLinkProgram(shaderProgram1);
    // link the program object 3
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // unsigned int VBOs[3], VAOs[3], EBOs[2];
    glGenVertexArrays(3, VAOs); // we can also generate multiple VAOs or buffers at the same time
    glGenBuffers(3, VBOs);
    glGenBuffers(2, EBOs);

    //  first
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexsTriangle), vertexsTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Vertex attributes stay the same
    glEnableVertexAttribArray(0);

    //  second
    glBindVertexArray(VAOs[1]);	// we bind to a different VAO now
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);	// and a different VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexsSquare), vertexsSquare, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesSquare), indicesSquare, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // because the vertex data is tightly packed we can also specify 0 as the vertex attribute's stride to let OpenGL figure it out
    glEnableVertexAttribArray(0);

    // third
    glBindVertexArray(VAOs[2]);	// we bind to a different VAO now
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);	// and a different VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexsRhombus), vertexsRhombus, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesRhombus), indicesRhombus, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // because the vertex data is tightly packed we can also specify 0 as the vertex attribute's stride to let OpenGL figure it out
    glEnableVertexAttribArray(0);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    menu();
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        if (a_basic == true) {
            // cout<<"var_scale "<<var_scale<<"\n";
            if ( var_scale >= 0.10 || var_scale <= -0.06) {
                // cout<<"enter\n";
                inc *= -1;
            }
            var_scale += inc;
            animation_basic();
        }
    
        // triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAOs[0]);

        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColorT");
        glUniform4f(vertexColorLocation, red_value, green_value, blue_value, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, 3);	

        // cuadrado
        glUseProgram(shaderProgram1);
        glBindVertexArray(VAOs[1]);
        int vertexColorLocation1 = glGetUniformLocation(shaderProgram1, "ourColorS");
        glUniform4f(vertexColorLocation1, red_value1, green_value1, blue_value1, 1.0f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	

        // rombo
        glUseProgram(shaderProgram2);
        glBindVertexArray(VAOs[2]);
        int vertexColorLocation2 = glGetUniformLocation(shaderProgram2, "ourColorR");
        glUniform4f(vertexColorLocation2, red_value2, green_value2, blue_value2, 1.0f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(3, VAOs);
    glDeleteBuffers(3, VBOs);
    glDeleteBuffers(2, EBOs);
    glDeleteProgram(shaderProgram);
    glDeleteProgram(shaderProgram1);
    glDeleteProgram(shaderProgram2);

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
    std::cout<<"    KEY LEFT: Traslación a la izquierda \n";
    std::cout<<"    KEY RIGHT: Traslación a la derecha \n";
    std::cout<<"    KEY UP: Traslación hacia arriba \n";
    std::cout<<"    KEY DOWN: Traslación hacia abajo \n";
    std::cout<<"    KEY R: Aparece y Colorea de rojo el rombo\n";
    std::cout<<"    KEY G: Aparece y Colorea de verde el rombo\n";
    std::cout<<"    KEY B: Aparece y Colorea de azul el rombo\n";
    std::cout<<"    KEY A: Animación básica (activa y desactiva la animación)\n";
    std::cout<<"    KEY I: Incrementa y disminuye la velocidad \n";
    std::cout<<"    KEY D: Incrementa y disminuye ángulo\n";
}

void Tshape_vertex(float full_vertex[cant_elem0], int it, float *shape_vtx)
{
    for (int i=0; i<=3; i++) {
        shape_vtx[i] = full_vertex[it]; 
        it++; 
    }
    shape_vtx[r_v-1] = 1.0f;
}

void Sshape_vertex(float full_vertex[cant_elem1], int it, float *shape_vtx)
{
    for (int i=0; i<=3; i++) {
        shape_vtx[i] = full_vertex[it]; 
        it++; 
    }
    shape_vtx[r_v-1] = 1.0f;
}

void Rshape_vertex(float full_vertex[cant_elem2], int it, float *shape_vtx)
{
    for (int i=0; i<=3; i++) {
        shape_vtx[i] = full_vertex[it]; 
        it++; 
    }
    shape_vtx[r_v-1] = 1.0f;
}

void render_matrix(Matrix &Mmain, Op &op, string option)
{
    if (option == shape_t) {
        for (int i = 0; i < cant_elem0; i+=3) {
            float shape_vtx[r_v] = {0,0,0,1.0};
            Tshape_vertex(vertexsTriangle, i, shape_vtx);
            Vector svtx(shape_vtx, r_v, c_v);

            Matrix r(Mmain.r, svtx.c);
            op.MultMatrixVector(svtx, r);
            vertexsTriangle[i] = r.M[0][0];
            vertexsTriangle[i+1] = r.M[1][0];
            vertexsTriangle[i+2] = r.M[2][0];
        }
        glBindVertexArray(VAOs[0]);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexsTriangle), vertexsTriangle, GL_STATIC_DRAW);
    }
    else if (option == shape_s) {
        for (int i = 0; i < cant_elem1; i+=3) {
            float shape_vtx[r_v] = {0,0,0,1.0};
            Sshape_vertex(vertexsSquare, i, shape_vtx);
            Vector svtx(shape_vtx, r_v, c_v);

            Matrix r(Mmain.r, svtx.c);
            op.MultMatrixVector(svtx, r);
            vertexsSquare[i] = r.M[0][0];
            vertexsSquare[i+1] = r.M[1][0];
            vertexsSquare[i+2] = r.M[2][0];
        }
        glBindVertexArray(VAOs[1]);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexsSquare), vertexsSquare, GL_STATIC_DRAW);
    }
    else if (option == shape_r) {
        // for c/vertex
        for (int i = 0; i < cant_elem2; i+=3) {
            float shape_vtx[r_v] = {0,0,0,1.0};
            Rshape_vertex(vertexsRhombus, i, shape_vtx);
            Vector svtx(shape_vtx, r_v, c_v);

            Matrix r(Mmain.r, svtx.c);
            op.MultMatrixVector(svtx, r);
            vertexsRhombus[i] = r.M[0][0];
            vertexsRhombus[i+1] = r.M[1][0];
            vertexsRhombus[i+2] = r.M[2][0];
        }
        glBindVertexArray(VAOs[2]);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexsRhombus), vertexsRhombus, GL_STATIC_DRAW);
    }
}
int r_count = 0;
int t_count = 0;
int td_count = 0;
void animation_basic ()
{
    while ( r_count < 8) {
        //square
        Matrix Mr_main(4,4);
        Mr_main.Identity();

        Op op(&Mr_main);
        op.Rotation_z(angle);
        render_matrix(Mr_main, op, "s");
        r_count++;
    }
    r_count = 0;
    while ( r_count<3 ) {
        r_count++;
    }

    Matrix Mt_main(4,4);
    Mt_main.Identity();
    Op t_op(&Mt_main);

    while (t_count < 14) {
        Matrix Mt_main(4,4);
        Mt_main.Identity();

        if ( t_count < 4) {
            // up
            t = var_tras;
            float ini[r_v] = {0.0f, t, 0.0f, 0.0f};
            Vector v(ini, 4,1);

            t_op.Traslation(v);
        }
        else if ( t_count > 4 && t_count < 7) {
            // left
            t = -var_tras;
            float ini[r_v] = {t, 0.0f, 0.0f, 0.0f};
            Vector v(ini, 4,1);

            t_op.Traslation(v);
            // render_matrix(Mt_main, t_op, "r");
        }
        else if ( t_count > 7 && t_count < 11) {
            // down
            t = -var_tras;
            float ini[r_v] = {0.0f, t, 0.0f, 0.0f};
            Vector v(ini, 4,1);

            t_op.Traslation(v);
            // render_matrix(Mt_main, t_op, "r");
        }
        else if ( t_count > 11 && t_count < 14) {
            // right
            t = var_tras;
            float ini[r_v] = {t, 0.0f, 0.0f, 0.0f};
            Vector v(ini, 4,1);

            t_op.Traslation(v);
            // render_matrix(Mt_main, t_op, "r");
        }      
        render_matrix(Mt_main, t_op, "r"); 
        t_count++;    
    }
    t_count = 0;
    while ( t_count<3 ) {
        t_count++;
    }

    //  triangle
    Matrix Ms_main(4,4);
    Ms_main.Identity();

    // cout<<"var into keys " <<var_scale<<"\n";
    float ini[r_v] = {1.0f + var_scale, 1.0f + var_scale, 1.0f + var_scale, 1.0f};
    Vector v(ini, 4,1);

    Op s_op(&Ms_main);
    s_op.Scale(v);
    render_matrix(Ms_main, s_op, "t");
}
//----------------------------------       KEYS         -----------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //  SCALE
    if( key == GLFW_KEY_1 && action == GLFW_PRESS || key == GLFW_KEY_1 && action == GLFW_REPEAT) {
        Matrix Mmain(4,4);
        Mmain.Identity();

        float ini[r_v] = {1.0f + var_scale, 1.0f + var_scale, 1.0f + var_scale, 1.0f};
        Vector v(ini, 4,1);

        Op op(&Mmain);
        op.Scale(v);
        render_matrix(Mmain, op, "t");
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS || key == GLFW_KEY_2 && action == GLFW_REPEAT)
    {
        Matrix Mmain(4,4);
        Mmain.Identity();

        float ini[r_v] = {1.0f - var_scale, 1.0f - var_scale, 1.0f - var_scale, 1.0f};
        Vector v(ini, 4,1);

        Op op(&Mmain);
        op.Scale(v);
        render_matrix(Mmain, op, "s");
    }

    //  ROTATION
    if (key == GLFW_KEY_Z && action == GLFW_PRESS || key == GLFW_KEY_Z && action == GLFW_REPEAT)
    {
        Matrix Mmain(4,4);
        Mmain.Identity();

        Op op(&Mmain);
        op.Rotation_z(angle);
        render_matrix(Mmain, op, "s");
    }

    if (key == GLFW_KEY_Y && action == GLFW_PRESS || key == GLFW_KEY_Y && action == GLFW_REPEAT)
    {
        Matrix Mmain(4,4);
        Mmain.Identity();

        Op op(&Mmain);
        op.Rotation_y(angle);
        render_matrix(Mmain, op, "s");
    }

    if (key == GLFW_KEY_X && action == GLFW_PRESS || key == GLFW_KEY_X && action == GLFW_REPEAT)
    {
        Matrix Mmain(4,4);
        Mmain.Identity();

        Op op(&Mmain);
        op.Rotation_x(angle);
        render_matrix(Mmain, op, "s");
    }

    //  TRASLATION
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS || key == GLFW_KEY_LEFT && action == GLFW_REPEAT)
    {
        Matrix Mmain(4,4);
        Mmain.Identity();

        float ini[r_v] = {-var_tras, 0.0f, 0.0f, 0.0f};
        Vector v(ini, 4,1);

        Op op(&Mmain);
        op.Traslation(v);
        render_matrix(Mmain, op, "r");
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS || key == GLFW_KEY_RIGHT && action == GLFW_REPEAT)
    {
        Matrix Mmain(4,4);
        Mmain.Identity();

        float ini[r_v] = {var_tras, 0.0f, 0.0f, 0.0f};
        Vector v(ini, 4,1);

        Op op(&Mmain);
        op.Traslation(v);
        render_matrix(Mmain, op, "r");
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS || key == GLFW_KEY_DOWN && action == GLFW_REPEAT)
    {
        Matrix Mmain(4,4);
        Mmain.Identity();

        float ini[r_v] = {0.0f, -var_tras, 0.0f, 0.0f};
        Vector v(ini, 4,1);

        Op op(&Mmain);
        op.Traslation(v);
        render_matrix(Mmain, op, "r"); 
    }
    if (key == GLFW_KEY_UP && action == GLFW_PRESS || key == GLFW_KEY_UP && action == GLFW_REPEAT)
    {
        Matrix Mmain(4,4);
        Mmain.Identity();

        float ini[r_v] = {0.0f, var_tras, 0.0f, 0.0f};
        Vector v(ini, 4,1);

        Op op(&Mmain);
        op.Traslation(v);
        render_matrix(Mmain, op, "r");   
    }

    // BASIC ANIMATION
    if ( i==0 && key == GLFW_KEY_A && action == GLFW_PRESS) {
            a_basic = true;     i+=1;
    }
    else if ( i==1 && key == GLFW_KEY_A && action == GLFW_PRESS) {
            a_basic = false;    i=0;
    }

    // color
    if (i==0 && key == GLFW_KEY_R && action == GLFW_PRESS) {
        red_value2 = 1.0;  i+=1;
    }
    else if (i==1 && key == GLFW_KEY_R && action == GLFW_PRESS) {
        red_value2 = 0.0; i=0;
    }

    else if (i==0 && key == GLFW_KEY_G && action == GLFW_PRESS) {
        green_value2 = 1.0;  i+=1;
    }
    else if (i==1 && key == GLFW_KEY_G && action == GLFW_PRESS) { 
        green_value2 = 0.0;  i=0;
    }

    else if (i==0 && key == GLFW_KEY_B && action == GLFW_PRESS) {
        blue_value2 = 1.0;   i+=1;
    }
    else if (i==1 && key == GLFW_KEY_B && action == GLFW_PRESS) {
        blue_value2 = 0.0;   i=0;
    }

    // incrementa y disminuye velocidad
    // if (i=0 && key == GLFW_KEY_I && action == GLFW_PRESS) {
    //     inc += 0.02; i=1;
    // }
    // else if (i=1 && key == GLFW_KEY_I && action == GLFW_PRESS) {
    //     inc -= 0.02; i=0;
    // }

    // incrementa y disminuye angulo
    // if (i=0 && key == GLFW_KEY_D && action == GLFW_PRESS) {
    //     angle += 1; i+=1;
    // }
    // if (i=1 && key == GLFW_KEY_D && action == GLFW_PRESS) {
    //     angle -= 1; i=0;
    // }
}
