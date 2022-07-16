#pragma once

#include "bracket_window.hpp"
#include "bracket_pipeline.hpp"
#include "bracket_device.hpp"

namespace bracket {
  class App {
    public:
      static constexpr int WIDTH = 800;
      static constexpr int HEIGHT = 600;

      void run();

    private:
      BracketWindow BracketWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
      BracketDevice bracketDevice{BracketWindow};
      BracketPipeline BracketPipeline{
        bracketDevice,
        "../src/shaders/simple_shader.vert.spv",
        "../src/shaders/simple_shader.frag.spv",
        BracketPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
  };
}