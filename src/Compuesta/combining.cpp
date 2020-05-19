/* 
        A CONSIDERAR
    En el control teórico: los ángulos no fueron convertidos
    a radianes(usado por funciones matemáticas), se olvidó; 
    pero cuando se trabaja con grados funciona de forma normal

*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "matrix.h"
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

// ------------------  Column L - R  -------------------------   
float degree_x = 77;
float degree_y = 49;
float degree_z = 77;

float translation_x = 0.27;
float translation_y = 0.49;
float translation_z = 0.35;

float scale_x = 1.25;
float scale_y = 1.25;
float scale_z = 1.0;
// -----------------------------------------------------------

float vert_triangle[] = {
    0.75f, -0.5f, 0.0f,  // right 
    0.25f, 0.5f, 0.0f,  // top
    0.0f, -0.5f, 0.0f,  // left 
};

const char *vert_trianglehaderSource = "#version 330 core\n"
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
    unsigned int vert_trianglehader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER); // the first fragment shader that outputs the color orange
    unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER); // the second fragment shader that outputs the color orange
    
    unsigned int shaderProgram = glCreateProgram();
    glShaderSource(vert_trianglehader, 1, &vert_trianglehaderSource, NULL);
    glCompileShader(vert_trianglehader);
    glShaderSource(fragmentShader1, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader1);

    // link the program object
    glAttachShader(shaderProgram, vert_trianglehader);
    glAttachShader(shaderProgram, fragmentShader1);
    glLinkProgram(shaderProgram);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO); // we can also generate multiple VAOs or buffers at the same time
    glGenBuffers(1, &VBO);

    // first triangle setup
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert_triangle), vert_triangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Vertex attributes stay the same
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

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);	// this call should output an triangle
    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

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
    std::cout<<"    KEY L: Combinación de tranformaciones columna L (S Rz T Ry-1) \n";
    std::cout<<"    KEY R: Combinación de tranformaciones columna R (Ry-1 Rz Rx T S Ry) \n";
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
        shape_vertex(vert_triangle, i, shape_vtx);
        Vector svtx(shape_vtx, r_v, c_v);

        Matrix r(Mmain.r, svtx.c);
        op.MultMatrixVector(svtx, r);
        vert_triangle[i] = r.M[0][0];
        vert_triangle[i+1] = r.M[1][0];
        vert_triangle[i+2] = r.M[2][0];

    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert_triangle), vert_triangle, GL_STATIC_DRAW);
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

    // --------------------------   Transformation column L-R
    // S Rz T Ry-1
    if (key == GLFW_KEY_L && action == GLFW_PRESS || key == GLFW_KEY_L && action == GLFW_REPEAT)
    {
        // S 
        Matrix Ms_main(4,4);
        Ms_main.Identity();

        float ini[r_v] = {0.0f + scale_x, 0.0f + scale_y, 0.0f + scale_z, 1.0f};
        Vector v(ini, 4,1);

        Op s_op(&Ms_main);
        s_op.Scale(v);
        cout << "\n     Scale:\n";
        p_rint_m(Ms_main);

        // Rz
        Matrix Mrz_main(4,4);
        Mrz_main.Identity();

        Op rz_op(&Mrz_main);
        rz_op.Rotation_z(degree_z);
        cout << "\n     Rotation z:\n";
        p_rint_m(Mrz_main);

        Matrix rSRz(Ms_main.r, Mrz_main.c);
        s_op.MultMatrixMatrix(Mrz_main, rSRz);
        cout << "\n     Multiplication Matrices S Rz\n";
        p_rint_m(rSRz);

        // T
        Matrix Mt_main(4,4);
        Mt_main.Identity();

        float t_ini[r_v] = {translation_x, translation_y, translation_z, 0.0f};
        Vector vt(t_ini, 4,1);

        Op t_op(&Mt_main);
        t_op.Traslation(vt);
        cout << "\n     Traslation\n";
        p_rint_m(Mt_main);

        Matrix rSRzT(Mrz_main.r, Mt_main.c);
        rz_op.mtz = &rSRz;
        rz_op.MultMatrixMatrix(Mt_main, rSRzT);
        cout << "\n     Multiplication Matrices S Rz T\n";
        p_rint_m(rSRzT);

        // Ry-1
        Matrix Mry_main(4,4);
        Mry_main.Identity();

        Op ry_op(&Mry_main);
        ry_op.Rotation_y(degree_y);
        cout << "\n     Rotation y:\n";
        p_rint_m(Mry_main);

        // Op adj_op(&Mry_main);
        // cout << "\n   The Adjoint is :\n"; 
        // Matrix adj(Mry_main.r, Mry_main.c);
        // adj_op.Adjoint(Mry_main, adj); 
        // p_rint_m(adj);
        Op op;
        Matrix inverse(Mry_main.r, Mry_main.c);
        op.MatrixInverse(Mry_main, inverse);
        cout << "\n     Matrix Inverse Ry-1:\n";
        p_rint_m(inverse);

        Matrix rSRzTRy_(Mt_main.r, inverse.c);
        t_op.mtz = &rSRzT;
        t_op.MultMatrixMatrix(inverse, rSRzTRy_);
        cout << "\n     Multiplication Matrices S Rz T Ry-1\n";
        p_rint_m(rSRzTRy_);

        render_matrix(rSRzTRy_, ry_op);
    }
    // Ry-1 Rz Rx T S Ry
    if (key == GLFW_KEY_R && action == GLFW_PRESS || key == GLFW_KEY_R && action == GLFW_REPEAT)
    {
        // Ry-1 
        Matrix Mry_main(4,4);
        Mry_main.Identity();

        Op ry_op(&Mry_main);
        ry_op.Rotation_y(degree_y);
        cout << "\n     Rotation y:\n";
        p_rint_m(Mry_main);

        // Op adj_op(&Mry_main);
        // cout << "\n   The Adjoint is :\n"; 
        // Matrix adj(Mry_main.r, Mry_main.c);
        // adj_op.Adjoint(Mry_main, adj); 
        // p_rint_m(adj);
        Op op;
        Matrix inverse(Mry_main.r, Mry_main.c);
        op.MatrixInverse(Mry_main, inverse);
        cout << "\n     Matrix Inverse Ry-1:\n";
        p_rint_m(inverse);

        // Rz
        Matrix Mrz_main(4,4);
        Mrz_main.Identity();

        Op rz_op(&Mrz_main);
        rz_op.Rotation_z(degree_z);
        cout << "\n     Rotation z:\n";
        p_rint_m(Mrz_main);

        Matrix rRy_Rz(Mry_main.r, Mrz_main.c);
        ry_op.mtz = &inverse;
        ry_op.MultMatrixMatrix(Mrz_main, rRy_Rz);
        cout << "\n     Multiplication Matrices Ry-1 Rz\n";
        p_rint_m(rRy_Rz);

        // Rx
        Matrix Mrx_main(4,4);
        Mrx_main.Identity();

        Op rx_op(&Mrx_main);
        rx_op.Rotation_z(degree_x);
        cout << "\n     Rotation x:\n";
        p_rint_m(Mrx_main);

        Matrix rRy_RzRx(Mrz_main.r, Mrx_main.c);
        rz_op.mtz = &rRy_Rz;
        rz_op.MultMatrixMatrix(Mrx_main, rRy_RzRx);
        cout << "\n     Multiplication Matrices Ry-1 Rz Rx\n";
        p_rint_m(rRy_RzRx);

        //T
        Matrix Mt_main(4,4);
        Mt_main.Identity();

        float ini[r_v] = {translation_x, translation_y, translation_z, 0.0f};
        Vector t_v(ini, 4,1);

        Op t_op(&Mt_main);
        t_op.Traslation(t_v);
        cout << "\n     Traslation x:\n";
        p_rint_m(Mt_main);

        Matrix rRy_RzRxT(Mrx_main.r, Mt_main.c);
        rx_op.mtz = &rRy_RzRx;
        rx_op.MultMatrixMatrix(Mt_main, rRy_RzRxT);
        cout << "\n     Multiplication Matrices Ry-1 Rz Rx T: \n";
        p_rint_m(rRy_RzRxT);

        // S
        Matrix Ms_main(4,4);
        Ms_main.Identity();

        float s_ini[r_v] = {0.0f + scale_x, 0.0f + scale_y, 0.0f + scale_z, 1.0f};
        Vector sv(s_ini, 4,1);

        Op s_op(&Ms_main);
        s_op.Scale(sv);
        cout << "\n     Scale:\n";
        p_rint_m(Ms_main);

        Matrix rRy_RzRxTS(Mt_main.r, Ms_main.c);
        t_op.mtz = &rRy_RzRxT;
        t_op.MultMatrixMatrix(Ms_main, rRy_RzRxTS);
        cout << "\n     Multiplication Matrices Ry-1 Rz Rx T S: \n";
        p_rint_m(rRy_RzRxTS);

        // Ry
        Matrix Mry(4,4);
        Mry.Identity();

        Op ry(&Mry);
        ry.Rotation_y(degree_y);
        cout << "\n     Rotation y:\n";
        p_rint_m(Mry);

        Matrix rRy_RzRxTSRy(Ms_main.r, Mry.c);
        s_op.mtz = &rRy_RzRxTS;
        s_op.MultMatrixMatrix(Mry, rRy_RzRxTSRy);
        cout << "\n     Multiplication Matrices Ry-1 Rz Rx T S Ry: \n";
        p_rint_m(rRy_RzRxTSRy);

        render_matrix(rRy_RzRxTSRy, ry);
    }
}