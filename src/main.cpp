

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

//Wireframe render
const bool wireframe_render = false;
// resolution
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// vertex data
const float vertices[] = {
     0.1f, 0.1f,0.0f,
     0.4f, 0.1f,0.0f,
     0.1f, 0.4f,0.0f,

    -0.1f,-0.1f,0.0f,
    -0.4f,-0.1f,0.0f,
    -0.1f,-0.4f,0.0f,
};
const unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,  // 1st Triangle
    1, 2, 3   // 2nd Triangle
};
    // ------------------------------------------------------------------

const char *vertexShaderSource = 
"#version 330 core\n"
"layout (location = 1) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char *fragmentShaderSource = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 0.3f);\n"
"}\n\0";

int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // ------------------------------

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // --------------------

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // ---------------------------------------

    // Check VAO capacity
    int maxAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributes);
    std::cout << "VAO capacity: " << maxAttributes << std::endl;
    // ------


    // build and compile shader program
    // COMPILE vertex shader
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

    // COMPILE fragment shader
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

    // LINK vertex + fragment shaders
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
    // ------------------------------------


    //Generate Verex Buffer Object
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    //Generate Vertex Array Object
    unsigned int VAO;
    unsigned int VAO_ab;
    glGenVertexArrays(1, &VAO);
    glGenVertexArrays(1, &VAO_ab);
    //Generate Entity Buffer Object
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    // VAO first
    glBindVertexArray(VAO);
    // Bind VBO and EBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // Fill VBO and EBO
    glBufferData(GL_ARRAY_BUFFER,         sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),   indices, GL_STATIC_DRAW);
    // ------

    // place info into binded VAO slot and enable it
    const int  VAO_index = 1;
    glVertexAttribPointer(VAO_index, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // VBO is saved into VAO slot
    glEnableVertexAttribArray(VAO_index);
    //Unbind the VAO
    glBindVertexArray(0); //Ubinding saves info in VAO
    // ------

    // Unbind VBO as VAO has info about it
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //unbind the EBO as VAO sealed info about it
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
    // ------
    

    //Setup way of render
    if(wireframe_render){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);
        // -----
        
        // Clearing up
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // ------
        
        // Set the shaders program
        glUseProgram(shaderProgram);
        //-------

        // Render
        // Bind the VAO
        glBindVertexArray(VAO);
        // Draw
        #if(1)
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glDrawArrays(GL_TRIANGLES, 3, 3);
        #else
            // or
            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        #endif
        //Unbind th VAO
        glBindVertexArray(0);
        // ------
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
        // -------------------------------------------------------------------------------
    }
    // -----------

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    // ------------------------------------------------------------------------

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
    // ------------------------------------------------------------------
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}
// ---------------------------------------------------------------------------------------------------------

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions
    glViewport(0, 0, width, height);
}

