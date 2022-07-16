#include "first_app.hpp"

namespace bracket {
  void App::run() {
    while (!BracketWindow.shouldClose()) {
      glfwPollEvents();
    }
  }
}