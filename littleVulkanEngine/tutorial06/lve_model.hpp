#pragma once

#include "lve_device.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE  // open gl uses -1 to 1, vk is 0 to 1
#include <glm/glm.hpp>

// std
#include <memory>
#include <vector>

namespace lve {

class LveModel {
 public:
  struct Vertex {
    glm::vec2 position;

    static std::vector<VkVertexInputBindingDescription> getBindingDescriptions() {
      std::vector<VkVertexInputBindingDescription> bindingDescriptions(
          1,
          VkVertexInputBindingDescription{});

      bindingDescriptions[0].binding = 0;
      bindingDescriptions[0].stride = sizeof(Vertex);
      bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
      return bindingDescriptions;
    }

    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
  };

  LveModel(LveDevice& device);
  ~LveModel() { cleanup(); }

  LveModel(const LveModel&) = delete;
  void operator=(const LveModel&) = delete;

  void draw(VkCommandBuffer commandBuffer);
  void bind(VkCommandBuffer commandBuffer);

 private:
  LveDevice& device;

  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  uint32_t vertexCount;
  std::vector<Vertex> vertices = {{{0.0f, -0.5f}}, {{0.5f, 0.5f}}, {{-0.5f, 0.5f}}};

  void createVertexBuffer();
  void cleanup();
};
}  // namespace lve