#pragma once

#include "bracket_device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace bracket {
  class BracketModel {
    public:

      struct Vertex {
        glm::vec2 position;
        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
      };
      BracketModel(BracketDevice &device, const std::vector<Vertex> &vertices);
      ~BracketModel();

      BracketModel(const BracketModel &) = delete;
      BracketModel &operator=(const BracketModel &) = delete;

      void bind(VkCommandBuffer commandBuffer);
      void draw(VkCommandBuffer commandBuffer);

    private:
      void createVertexBuffers(const std::vector<Vertex> &vertices);

      BracketDevice &bracketDevice;
      VkBuffer vertexBuffer;
      VkDeviceMemory vertexBufferMemory;
      uint32_t vertexCount;
  };
}