#include "first_app.hpp"

#include <stdexcept>
#include <array>

namespace bracket {

  App::App() {
    loadModels();
    createPipelineLayout();
    createPipeline();
    createCommandBuffers();
  }

  App::~App() {
    vkDestroyPipelineLayout(bracketDevice.device(), pipelineLayout, nullptr);
  }

  void App::run() {
    while (!bracketWindow.shouldClose()) {
      glfwPollEvents();
      drawFrame();
    }

    vkDeviceWaitIdle(bracketDevice.device());
  }

  void App::loadModels() {
    std::vector<BracketModel::Vertex> vertices{};
    sierpinski(vertices, 7, {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.0f, -0.5f});

    bracketModel = std::make_unique<BracketModel>(bracketDevice, vertices);
  }

  void App::sierpinski(
    std::vector<BracketModel::Vertex> &vertices,
    int depth,
    glm::vec2 left,
    glm::vec2 right,
    glm::vec2 top) {
    if (depth <= 0) {
      vertices.push_back({top});
      vertices.push_back({right});
      vertices.push_back({left});
    } else {
      auto leftTop = 0.5f * (left + top);
      auto rightTop = 0.5f * (right + top);
      auto leftRight = 0.5f * (left + right);
      sierpinski(vertices, depth - 1, left, leftRight, leftTop);
      sierpinski(vertices, depth - 1, leftRight, right, rightTop);
      sierpinski(vertices, depth - 1, leftTop, rightTop, top);
    }
  }

  void App::createPipelineLayout() {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;
    if (vkCreatePipelineLayout(bracketDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
      throw std::runtime_error("failed to create pipeline layout");
    }
  }

  void App::createPipeline() {
    auto pipelineConfig = BracketPipeline::defaultPipelineConfigInfo(bracketSwapChain.width(), bracketSwapChain.height());
    pipelineConfig.renderPass = bracketSwapChain.getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    bracketPipeline = std::make_unique<BracketPipeline>(
        bracketDevice,
        "../src/shaders/simple_shader.vert.spv",
        "../src/shaders/simple_shader.frag.spv",
        pipelineConfig);
  }

  void App::createCommandBuffers() {
    commandBuffers.resize(bracketSwapChain.imageCount());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = bracketDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(bracketDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
      throw std::runtime_error("failed to allocate command buffers!");
    }

    for (int i = 0; i < commandBuffers.size(); i++) {
      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
      if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
      }

      VkRenderPassBeginInfo renderPassInfo{};
      renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      renderPassInfo.renderPass = bracketSwapChain.getRenderPass();
      renderPassInfo.framebuffer = bracketSwapChain.getFrameBuffer(i);

      renderPassInfo.renderArea.offset = {0, 0};
      renderPassInfo.renderArea.extent = bracketSwapChain.getSwapChainExtent();

      std::array<VkClearValue, 2> clearValues{};
      clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
      clearValues[1].depthStencil = {1.0f, 0};
      renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
      renderPassInfo.pClearValues = clearValues.data();

      vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

      bracketPipeline->bind(commandBuffers[i]);
      bracketModel->bind(commandBuffers[i]);
      bracketModel->draw(commandBuffers[i]);

      vkCmdEndRenderPass(commandBuffers[i]);
      if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
      }
    }
  }
  void App::drawFrame() {
    uint32_t imageIndex;
    auto result = bracketSwapChain.acquireNextImage(&imageIndex);

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      throw std::runtime_error("failed to acquire swap chain image!");
    }

    result = bracketSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
    if (result != VK_SUCCESS) {
      throw std::runtime_error("failed to present swap chain image!");
    }
  }
}