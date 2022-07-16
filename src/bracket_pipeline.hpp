#pragma once

#include "bracket_device.hpp"

#include <string>
#include <vector>

namespace bracket {

  struct PipelineConfigInfo {};

  class BracketPipeline {
    public:
      BracketPipeline(
        BracketDevice &device,
        const std::string &vertFilepath, 
        const std::string &fragFilepath,
        const PipelineConfigInfo& configInfo);

      ~BracketPipeline(){};

      BracketPipeline(const BracketPipeline&) = delete;
      void operator=(const BracketPipeline&) = delete;

      static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

    private:
      static std::vector<char> readFile(const std::string &filePath);
      void createGraphicsPipeline(
        const std::string &vertFilepath,
        const std::string &fragFilepath,
        const PipelineConfigInfo& configInfo);

      void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

      BracketDevice& bracketDevice;
      VkPipeline graphicsPipeline;
      VkShaderModule vertShaderModule;
      VkShaderModule fragShaderModule;
  };
}