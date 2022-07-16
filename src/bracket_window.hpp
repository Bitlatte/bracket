#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace bracket {
  class BracketWindow {
    public:
      BracketWindow(int w, int h, std::string name);
      ~BracketWindow();

      BracketWindow(const BracketWindow &) = delete;
      BracketWindow &operator=(const BracketWindow &) = delete;

      bool shouldClose() { return glfwWindowShouldClose(window); }

      void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

    private:
      void initWindow();

      const int width;
      const int height;

      std::string windowName;
      GLFWwindow *window;
  };
}