#include "bracket_pipeline.hpp"

#include <fstream>
#include <stdexcept>

#include <iostream>

namespace bracket {

  BracketPipeline::BracketPipeline(
        BracketDevice &device,
        const std::string &vertFilepath, 
        const std::string &fragFilepath,
        const PipelineConfigInfo& configInfo)
        : bracketDevice{device} {
    createGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
  }

  std::vector<char> BracketPipeline::readFile(const std::string& filePath) {
    std::ifstream file{filePath, std::ios::ate | std::ios::binary};

    if (!file.is_open()) {
      throw std::runtime_error("failed to open file: " + filePath);
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();
    return buffer;
  }

  void BracketPipeline::createGraphicsPipeline(
    const std::string &vertFilepath,
    const std::string &fragFilepath,
    const PipelineConfigInfo& configInfo) {
    auto vertCode = readFile(vertFilepath);
    auto fragCode = readFile(vertFilepath);

    std::cout << "Vertex Shader Code Size: " << vertCode.size() << std::endl;
    std::cout << "Fragment Shader Code Size: " << fragCode.size() << std::endl;
  }

  void BracketPipeline::createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    if (vkCreateShaderModule(bracketDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
      throw std::runtime_error("failed to create shader module");
    }
  }

  PipelineConfigInfo BracketPipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height) {
    PipelineConfigInfo configInfo{};

    return configInfo;
  }

}