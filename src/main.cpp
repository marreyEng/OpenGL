

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

//Wireframe render
const bool wireframe = false;
// resolution
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// vertex data
const float vertices_a[] = {
     0.1f, 0.1f,0.0f,
     0.4f, 0.1f,0.0f,
     0.1f, 0.4f,0.0f
};
const float vertices_b[] = {
    -0.1f,-0.1f,0.0f,
    -0.4f,-0.1f,0.0f,
    -0.1f,-0.4f,0.0f,
};

const char *vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
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
    unsigned int VBO_a, VBO_b;
    glGenBuffers(1, &VBO_a);
    glGenBuffers(1, &VBO_b);

    //Generate Vertex Array Object
    unsigned int VAO_a, VAO_b;
    glGenVertexArrays(1, &VAO_a);
    glGenVertexArrays(1, &VAO_b);
    //Generate Entity Buffer Object

    // VAO first
    glBindVertexArray(VAO_a);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_a);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_a), vertices_a, GL_STATIC_DRAW);
    const int  VAO_index_a = 0;
    glVertexAttribPointer(VAO_index_a, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0); // VBO is saved into VAO slot
    glEnableVertexAttribArray(VAO_index_a);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(VAO_b);   
    glBindBuffer(GL_ARRAY_BUFFER, VBO_b);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_b), vertices_b, GL_STATIC_DRAW);
    const int VAO_index_b = 0;
    glVertexAttribPointer(VAO_index_b, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(VAO_index_b);
    glBindVertexArray(0); //Ubinding saves info in VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    //Setup wireframe 
    if(wireframe){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);
        // Cleaning up
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Set the shaders program
        glUseProgram(shaderProgram);


        // Render
        glBindVertexArray(VAO_a);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(VAO_b);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        // ------
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
        // -------------------------------------------------------------------------------
    }
    // -----------

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO_a);
    glDeleteVertexArrays(1, &VAO_b);
    glDeleteBuffers(1, &VBO_a);
    glDeleteBuffers(1, &VBO_b);
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

