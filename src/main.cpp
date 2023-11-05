#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

#include "Renderer.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

#include "Shader.h"
#include "Texture.h"

const int WIDTH = 800;
const int HEIGHT = 600;

int main() {  
  if (!glfwInit()) {
    std::cout << "GLFW initialization failed!" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Set OpenGL version to 3.x
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Set OpenGL version to x.3
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use core profile

  // Create a GLFW window
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Window", nullptr, nullptr);

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

  {
    float positions[] = {
      -0.5f, -0.5f, 0.0f, 0.0f, // 0
       0.5f, -0.5f, 1.0f, 0.0f, // 1
       0.5f,  0.5f, 1.0f, 1.0f, // 2
      -0.5f,  0.5f, 0.0f, 1.0f  // 3
    };

    unsigned int indices[] = {
      0, 1, 2,
      2, 3, 0
    };

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));
    
    VertexBufferLayout layout;
    layout.Push(GL_FLOAT, 2); // position
    layout.Push(GL_FLOAT, 2); // texture coordinates

    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    glm::mat4 proj = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    Shader shader("res/shaders/basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

    Texture texture("res/textures/test.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);

    // Unbind everything
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();
    
    Renderer renderer;

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();
    
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    float r = 0.0f;
    float increment = 0.05f;

    glm::vec2 speedBall(1.0f, 1.0f);

    glm::vec3 translationBall(WIDTH / 2, HEIGHT / 2, 0);
    glm::vec3 translationLeft(100, HEIGHT / 2, 0);
    glm::vec3 translationRight(WIDTH - 100, HEIGHT / 2, 0);

    glm::vec3 scaleBall(25, 25, 0);
    glm::vec3 scaleLeft(25, 100, 0);
    glm::vec3 scaleRight(25, 100, 0);

    std::cout << "Entering main loop..." << std::endl;

    // Main loop until the window is closed
    while (!glfwWindowShouldClose(window)) {
      renderer.Clear();

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
      
      {
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), scaleLeft);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translationLeft);
        glm::mat4 mvp = proj * view * model * scale;

        shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
        shader.SetUniformMat4f("u_MVP", mvp);
        
        renderer.Draw(va, ib, shader);
      }
      
      {
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), scaleRight);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translationRight);
        glm::mat4 mvp = proj * view * model * scale;

        shader.Bind();
        shader.SetUniformMat4f("u_MVP", mvp);

        renderer.Draw(va, ib, shader);
      }
      
      {
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), scaleBall);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translationBall);
        glm::mat4 mvp = proj * view * model * scale;

        shader.Bind();
        shader.SetUniformMat4f("u_MVP", mvp);

        renderer.Draw(va, ib, shader);
      }

      if (r > 1.0f) {
        increment = -0.05f;
      } else if (r < 0.0f) {
        increment = 0.05f;
      }

      r += increment;

      // ball movement
      translationBall.x += speedBall.x;
      translationBall.y += speedBall.y;

      if (translationBall.x + scaleBall.x / 2 >= WIDTH || translationBall.x - scaleBall.x / 2 <= 0) {
        speedBall.x = -speedBall.x;
      }

      if (translationBall.y + scaleBall.y / 2 >= HEIGHT || translationBall.y - scaleBall.y / 2 <= 0) {
        speedBall.y = -speedBall.y;
      }

      // left paddle movement
      if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        translationLeft.y += 5;
      }

      if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        translationLeft.y -= 5;
      }

      // right paddle movement
      if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        translationRight.y += 5;
      }

      if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        translationRight.y -= 5;
      }

      // Collision detection

      // left paddle
      if (translationBall.x - scaleBall.x / 2 <= translationLeft.x + scaleLeft.x / 2 &&
          translationBall.y - scaleBall.y / 2 <= translationLeft.y + scaleLeft.y / 2 &&
          translationBall.y + scaleBall.y / 2 >= translationLeft.y - scaleLeft.y / 2) {
        speedBall.x = -speedBall.x;
      }

      // right paddle
      if (translationBall.x + scaleBall.x / 2 >= translationRight.x - scaleRight.x / 2 &&
          translationBall.y - scaleBall.y / 2 <= translationRight.y + scaleRight.y / 2 &&
          translationBall.y + scaleBall.y / 2 >= translationRight.y - scaleRight.y / 2) {
        speedBall.x = -speedBall.x;
      }

      // {
      //   ImGui::Begin("Testing");

      //   ImGui::Text("This is some useful text.");
      //   ImGui::SliderFloat2("Translation A", &translationLeft.x, 0.0f, 960.0f);
      //   ImGui::SliderFloat2("Translation B", &translationRight.x, 0.0f, 960.0f);

      //   ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
      //   ImGui::End();
      // }

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      
      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  std::cout << "Terminating GLFW..." << std::endl;
  glfwTerminate();
  return 0;
}
