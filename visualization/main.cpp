#include<iostream>
#include<fstream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<string.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>


using namespace std;

// window dimensions
const GLint WIDTH = 1200, HEIGHT = 800;
GLuint VAO[2], VBO[2], IBO, shader, uniform_color;

// Vertex Shader
static const char* vShader = "                                                 \n\
#version 330                                                                   \n\
layout (location = 0) in vec3 pos;                                             \n\
uniform mat4 projection;					               \n\
uniform mat4 view;						               \n\
uniform mat4 model;						               \n\
void main()                                                                    \n\
{                                                                              \n\
    gl_Position = projection * view * model * vec4(pos.x, pos.y, pos.z, 1.0f); \n\
                                                                               \n\
}                                                                      	       \n\
";

static const char* fShader = "        \n\
#version 330                          \n\
uniform vec3 col;                     \n\
out vec4 colour;                      \n\
void main()                           \n\
{                                     \n\
    colour = vec4(col, 1.0);          \n\
}                                     \n\
";

GLfloat cube_vertices[] = { 
		       1.0f,  1.0f,  1.0f,  // Top right
	              -1.0f,  1.0f,  1.0f,  // Top left
		      -1.0f, -1.0f,  1.0f,  // bottom left
                       1.0f, -1.0f,  1.0f,  // bottom right 
                       1.0f,  1.0f, -1.0f,  //
                      -1.0f,  1.0f, -1.0f,  // Back
                      -1.0f, -1.0f, -1.0f,  //
                       1.0f, -1.0f, -1.0f };


// index array of vertex array for glDrawElements() & glDrawRangeElement()
GLuint indices[]  = {  0, 1,
		       1, 2,
                       2, 3,     
                       3, 0,
                       4, 5,
                       5, 6,     
                       6, 7,
		       7, 4,
                       0, 4,
                       1, 5,     
                       2, 6,
		       3, 7 };

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar elog[1024] = {0};

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if(!result)
    {
        glGetShaderInfoLog(theShader, sizeof(elog), NULL, elog);
        cout << "Error Compiling the " << shaderType << " Shader!" << elog << endl;
    }

    glAttachShader(theProgram, theShader);
}

void CompileShaders()
{
    shader = glCreateProgram();

    if(!shader)
    {
        cout << "Error creating shader program!" << endl;
    }

    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar elog[1024] = {0};

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if(!result)
    {
        glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
        cout << "Error linking program!" << elog << endl;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if(!result)
    {
        glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
        cout << "Error validating program!" << elog << endl;
    }

    uniform_color = glGetUniformLocation(shader, "col");

}


void CreatePoints(float vert[], unsigned int s)
{

    glGenVertexArrays(1, &VAO[0]);
    glBindVertexArray(VAO[0]);

    glGenBuffers(1, &VBO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert) * s, vert, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void CreateCube()
{


    glGenVertexArrays(1, &VAO[1]);
    glBindVertexArray(VAO[1]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glGenBuffers(1, &VBO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

int main()
{
    // Initialize GLFW
    if(!glfwInit())
    {
        cout << "GLFW initialization failed!" << endl;
        glfwTerminate();
        return 1;
    }

    ifstream in;
    in.open("data.dat");
    unsigned int n_vert{0};
    unsigned int n_points{0};
    in >> n_points;
    n_vert = 3 * n_points;
    float *point_vertices = new float[n_vert];
    int i = 0;
    float x{0.};

    while(in >> x)
    {
	point_vertices[i] = x;
     	i++;
    }
    in.close();

    // Setup GLFW window properties
    // OpenGL version

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Core profile = No Backwards Compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT,
                                               "Pi Estimation", NULL, NULL);
    if(!mainWindow)
    {
        cout << "GLFW window creation failed!" << endl;
        glfwTerminate();
        return 1;
    }


    // Get Buffer size information
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK)
    {
        cout << "GLEW initialization failed!" << endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // Setup Viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    CreatePoints(point_vertices, n_vert);
    CreateCube();
    CompileShaders();
    
    glEnable(GL_DEPTH_TEST);

    // Loop until window closed
    while(!glfwWindowShouldClose(mainWindow))
    {
        // Get and Handle user input events
        glfwPollEvents();

        // Clear window
        glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader);

        glm::mat4 view;
        glm::mat4 projection;
	glm::mat4 model;

        view = glm::lookAt(glm::vec3(0.0f, 0.0f, 6.0f), 
  		           glm::vec3(0.0f, 0.0f, 0.0f), 
  		           glm::vec3(0.0f, 1.0f, 0.0f));

	projection = glm::perspective(glm::radians(45.0f), (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);
	model = glm::rotate(model, (float)glfwGetTime()/10.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        //projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.1f, 10.0f);

        GLint viewLoc = glGetUniformLocation(shader, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	GLint projectionLoc = glGetUniformLocation(shader, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glUniform3f(uniform_color, 0.0f, 0.0f, 1.0f);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_POINTS, 0, n_vert);
        glBindVertexArray(0);

	glUniform3f(uniform_color, 1.0f, 1.0f, 1.0f);
	glBindVertexArray(VAO[1]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, indices);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    delete[]point_vertices;

    return 0;
}


