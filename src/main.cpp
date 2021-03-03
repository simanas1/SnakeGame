#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "Grid.h"

/* //testing memory allocations
void* operator new(size_t size)
{
    std::cout << "allocating: " << size << "bytes\n";
    return malloc(size);
}

void operator delete(void* memory, size_t size)
{
    std::cout << "freeing: " << size << "bytes\n";
    free(memory);
}
*/

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);



//TODO: create shader class
//change vertex positions to be based on pixels in screen
glm::mat4 Projection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, -1.0f, 1.0f);
//change draw start position to be bot left
glm::mat4 Translation = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

// create transformations
glm::mat4 model = Projection * Translation;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec4 aColor;\n"
"uniform mat4 transform;\n"
"out vec4 vColor;\n"
"void main()\n"
"{\n"
"   vColor = aColor;\n"
"   gl_Position = transform * vec4(aPos, 1.0f);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vColor;\n"
"void main()\n"
"{\n"
"   FragColor = vColor;\n"
"}\n\0";

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
   

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SnakeGame", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    /* Initialize GLEW - Modern OpenGL */
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        std::cout << glewGetErrorString(err) << '\n';

    }
    std::cout << glGetString(GL_VERSION) << '\n';


    //TODO: create shader class
    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
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
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
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
    unsigned int shaderProgram = glCreateProgram();
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
    
    //Create grid
    std::unique_ptr<Grid> grid = std::make_unique<Grid>(SCR_WIDTH,SCR_HEIGHT);;

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    double seconds = 0.0;
    double moveSeconds = 0.0;
    double appleSeconds = 0.0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        seconds = glfwGetTime();
        //keyboard input
        processInput(window);

        
        
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS )
            grid->m_Snake->changeDirection(left);
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            grid->m_Snake->changeDirection(right);
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            grid->m_Snake->changeDirection(up);
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            grid->m_Snake->changeDirection(down);

        if (seconds - moveSeconds > 0.08 && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
        {
            moveSeconds = seconds;
            grid->m_Snake->Move();
        }


        //TODO: if apple is eaten - timer is not reset, maybe move to Grid.cpp. Disabled currently
        /*
        //resets apple to different location periodically
        if (seconds - appleSeconds > 15.0)
        {
            appleSeconds = seconds;
            grid->PlaceApple();
        }
        */

        //updated grid
        grid->Update();

        //transform
        glm::mat4 transform = model;

        //draw
        glUseProgram(shaderProgram);

        //TODO: cache glGetUniformLocation somehow
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
 
        grid->Draw();
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------

    glDeleteProgram(shaderProgram);

    glfwTerminate();

    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}