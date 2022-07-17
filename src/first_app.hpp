#pragma once

#include "bracket_window.hpp"
#include "bracket_pipeline.hpp"
#include "bracket_device.hpp"
#include "bracket_swap_chain.hpp"

#include <memory>
#include <vector>

namespace bracket {
  class App {
    public:
      static constexpr int WIDTH = 800;
      static constexpr int HEIGHT = 600;

      App();
      ~App();

      App(const App &) = delete;
      App &operator=(const App &) = delete;

      void run();

    private:
      void createPipelineLayout();
      void createPipeline();
      void createCommandBuffers();
      void drawFrame();

      BracketWindow bracketWindow{WIDTH, HEIGHT, "Bracket Studio"};
      BracketDevice bracketDevice{bracketWindow};
      BracketSwapChain bracketSwapChain{bracketDevice, bracketWindow.getExtent()};
      std::unique_ptr<BracketPipeline> bracketPipeline;
      VkPipelineLayout pipelineLayout;
      std::vector<VkCommandBuffer> commandBuffers;
  };
}