#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <vector>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

class Vertice{
    public:
    float x, y, z;
    Vertice(){};
    Vertice(float _x, float _y, float _z){
        this->x=_x;  this->y=_y;  this->z=_z;
    };
};    

int red_value=0, green_value=0, blue_value=0; 
// float colors[] = {red_value, green_value, blue_value};
int i=0;
int vertexColorLocation;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    // "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    // "uniform vec4 aColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    // "   ourColor = aColor;\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    // "in vec3 ourColor;\n"
    "uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    // "   FragColor = vec4(ourColor, 1.0f);\n"
    "   FragColor = ourColor;\n"
    "}\n\0";

Vertice Get_Middle(Vertice p1,Vertice p2)
{
    float x=(p1.x + p2.x) / 2.0f;
    float y=(p1.y + p2.y) / 2.0f;
    float z=(p1.z + p2.z) / 2.0f;
    Vertice middle(x,y,z);
    return middle;
}

void Shape_triangle(Vertice points[], Vertice v1, Vertice v2, Vertice v3)
{
    std::cout<<"***** MOSTRAR TRIANGULO *****\n";
    static int i = 0;

    points[i++] = v1;
    points[i++] = v2;
    points[i++] = v3;

}

void Sierpinski(Vertice points[], Vertice v1, Vertice v2, Vertice v3, int degree, int &shader)
{
    if (degree > 0) {
        Vertice aux_vert[3];
        std::cout<<"degree!!"<<degree<<"\n";
        aux_vert[0]=Get_Middle(v1, v2);
        aux_vert[1]=Get_Middle(v1, v3);
        aux_vert[2]=Get_Middle(v2, v3);

        std::cout<<"1s-- " <<  " \n";
        Sierpinski(points, v1, aux_vert[0], aux_vert[1], degree-1,shader);
        std::cout<<"2s-- " <<  "\n";
        Sierpinski(points, v3, aux_vert[1], aux_vert[2], degree-1,shader);          
        std::cout<<"3s-- " <<"\n";
        Sierpinski(points, v2, aux_vert[2], aux_vert[0], degree-1,shader);
    }
    else {
        Shape_triangle(points, v1, v2, v3);
    }
}
int degree=5;
int nro_vert=int(pow(3, degree + 1));
float valcolor=0.8;
float inc = 0.004;

void Render(int opcion)
{
    if ( opcion == 1 )
        glUniform4f(vertexColorLocation, valcolor, 0.3f, 0.7f, 1.0f);
    else if ( opcion == 2 )
        glUniform4f(vertexColorLocation, 0.8f, 0.5f, valcolor, 1.0f);
    else if (opcion == 3 )
        glUniform4f(vertexColorLocation, 0.2f, valcolor, 0.4, 1.0f);
    else
        glUniform4f(vertexColorLocation, 0.0f, 0.0f, 0.0f, 1.0f);
}

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


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    Vertice vertices[] = {
        Vertice(-0.5f, -0.5f, 0.0f),   
        Vertice(0.0f,  0.5f, 0.0f), 
        Vertice(0.5f, -0.5f, 0.0f)
    };

    Vertice points[nro_vert];
    points[0] = Vertice(0.25, 0.5,0);

    std::cout<<"                DRAW SIERPINSKI\n";
    Sierpinski(points, vertices[0], vertices[1], vertices[2], degree, shaderProgram);

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    // position 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    glUseProgram(shaderProgram);

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    std::cout<<sizeof(points)<<"\n";
    std::cout<<sizeof(points)/9<<"\n";
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw triangle
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organize

        
        if (valcolor >= 0.9 || valcolor <= 0.3) {
            inc *= -1;
        }
        valcolor += inc;
        vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor"); 
        // glUniform4f(vertexColorLocation, 0.35f, green_value, 0.75f, 1.0f);
        glUniform4f(vertexColorLocation, 0.0f, valcolor, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, nro_vert/3);

        // vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor"); 
        glUniform4f(vertexColorLocation, 0.35f, valcolor, 0.5f, 1.0f);
        glDrawArrays(GL_TRIANGLES, nro_vert/3, nro_vert-(nro_vert/3));

        // vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor"); 
        glUniform4f(vertexColorLocation, 0.6f, 0.2, valcolor, 1.0f);
        glDrawArrays(GL_TRIANGLES, nro_vert-(nro_vert/3), nro_vert);
        
        for(int i=0; i<nro_vert/3; i+=3) {
            Render(red_value);
            glDrawArrays(GL_TRIANGLES, i, 3);
        }
        for(int i=nro_vert/3; i<nro_vert-(nro_vert/3); i+=3) {
            Render(green_value);
            glDrawArrays(GL_TRIANGLES, i, 3);
        }
        for(int i=nro_vert-(nro_vert/3); i<nro_vert; i+=3) {
            Render(blue_value);
            glDrawArrays(GL_TRIANGLES, i, 3);
        }
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // float timeValue = glfwGetTime();
        // float greenValue = sin(timeValue) / 2.0f + 0.5f;

        // glDrawElements(GL_TRIANGLES, sizeof(points), GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time 
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources
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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
        red_value = green_value = blue_value = 0; 
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        red_value = (red_value + 3) % 4;
        green_value = (red_value + 1) % 4;
        blue_value = (green_value + 1) % 4;
    }
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
        red_value = (red_value + 1)%4;
        green_value = (red_value + 1)%4;
        blue_value = (green_value + 1)%4;
    }
}