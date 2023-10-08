#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
  x;\
  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError() {
  while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line) {
  while (GLenum error = glGetError()) {
    std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
    return false;
  }

  return true;
}
struct ShaderProgramSource {
  std::string VertexSource;
  std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) {
  std::ifstream stream(filepath);

  enum class ShaderType {
    NONE = -1,
    VERTEX = 0,
    FRAGMENT = 1
  };

  std::string line;
  std::stringstream ss[2];
  ShaderType type = ShaderType::NONE;

  while (getline(stream, line)) {
    if (line.find("#shader") != std::string::npos) {
      if (line.find("vertex") != std::string::npos) {
        type = ShaderType::VERTEX;
      } else if (line.find("fragment") != std::string::npos) {
        type = ShaderType::FRAGMENT;
      }
    } else {
      ss[(int)type] << line << '\n';
    }
  }

  return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
  unsigned int id = glCreateShader(type);
  const char* src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);

  if (result == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char* message = (char*)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
    std::cout << message << std::endl;
    glDeleteShader(id);
    return 0;
  }

  return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

int main() {  
  if (!glfwInit()) {
    std::cout << "GLFW initialization failed!" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Set OpenGL version to 3.x
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Set OpenGL version to x.3
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use core profile

  // Create a GLFW window
  GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", nullptr, nullptr);

  if (!window) {
    std::cout << "GLFW window creation failed!" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync or sync with monitor refresh rate

  if (glewInit() != GLEW_OK) {
    std::cout << "GLEW initialization failed!" << std::endl;
    return -1;
  }

  // Log OpenGL version
  std::cout << glGetString(GL_VERSION) << std::endl;

  float positions[] = {
    -0.5f, -0.5f, // 0
     0.5f, -0.5f, // 1
     0.5f,  0.5f, // 2
    -0.5f,  0.5f, // 3
  };

  unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
  };

  // Now that we are using the core profile we need to create a vertex array object
  // This was previously created in the background by GLFW
  unsigned int vao; // Vertex array object
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create a buffer to store the vertex data (positions)
  // This is the buffer that will hold the vertex data
  unsigned int buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

  // Create a buffer to store the index data, this way we don't have to send the same vertex data multiple times
  // This is the buffer that will hold the index data pointing to the vertex data
  unsigned int ibo; // Index buffer object
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

  // Tell OpenGL how to interpret the vertex data
  int attributeIndex = 0; // We only have one attribute, the position
  // Enable the vertex attribute array
  glEnableVertexAttribArray(attributeIndex);
  // Next we need to tell OpenGL how to interpret the vertex data
  // This is because our vertex data is just a bunch of floats, OpenGL doesn't know how to interpret it
  // We could have a bunch of floats that represent colors, or texture coordinates, or normals, etc. In this case we only have positions
  glVertexAttribPointer(attributeIndex, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0); // This is the line that links the buffer to the vertex array object

  ShaderProgramSource source = ParseShader("res/shaders/basic.shader");
  // std::cout << "VERTEX" << std::endl;
  // std::cout << source.VertexSource << std::endl;
  // std::cout << "FRAGMENT" << std::endl;
  // std::cout << source.FragmentSource << std::endl;

  unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
  glUseProgram(shader);

  int location = glGetUniformLocation(shader, "u_Color");
  ASSERT(location != -1);
  glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f);

  // Unbind everything
  glBindVertexArray(0);
  glUseProgram(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  float r = 0.0f;
  float increment = 0.05f;

  // Main loop until the window is closed
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    // Before we draw anything we need to bind the shader and the vertex array object
    glUseProgram(shader);
    GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
    
    // As we are usinig an index buffer we no longer need to bind the vertex buffer because the vertex array object already has a reference to it
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

    if (r > 1.0f) {
      increment = -0.05f;
    } else if (r < 0.0f) {
      increment = 0.05f;
    }

    r += increment;

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteProgram(shader);

  glfwTerminate();
  return 0;
}
