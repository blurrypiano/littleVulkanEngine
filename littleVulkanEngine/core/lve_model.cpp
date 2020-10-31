#include "lve_model.hpp"

// lib headers
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

// std
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

namespace std {
template <>
struct hash<lve::LveModel::Vertex> {
  size_t operator()(lve::LveModel::Vertex const& vertex) const {
    return ((hash<glm::vec3>()(vertex.position) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
           (hash<glm::vec2>()(vertex.uv) << 1) ^ (hash<glm::vec3>()(vertex.normal) << 1) ^
           (hash<glm::vec3>()(vertex.tangent) << 1);
  }
};
}  // namespace std

namespace lve {
LveModel::LveModel(LveDevice& device, Builder& builder) : device_{device} {
  createVertexBuffer(builder);
  createIndexBuffer(builder);
}

void LveModel::createVertexBuffer(Builder& builder) {
  auto& vertices = builder.vertices;
  vertexCount_ = static_cast<uint32_t>(vertices.size());
  VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount_;

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  device_.createBuffer(
      bufferSize,
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
      stagingBuffer,
      stagingBufferMemory);

  void* data;
  vkMapMemory(device_.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, vertices.data(), (size_t)bufferSize);
  vkUnmapMemory(device_.device(), stagingBufferMemory);

  device_.createBuffer(
      bufferSize,
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
      vertexBuffer_,
      vertexBufferMemory_);

  device_.copyBuffer(stagingBuffer, vertexBuffer_, bufferSize);

  vkDestroyBuffer(device_.device(), stagingBuffer, nullptr);
  vkFreeMemory(device_.device(), stagingBufferMemory, nullptr);
}

void LveModel::createIndexBuffer(Builder& builder) {
  auto& indices = builder.indices;
  indexCount_ = static_cast<uint32_t>(indices.size());
  VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount_;

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;

  device_.createBuffer(
      bufferSize,
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
      stagingBuffer,
      stagingBufferMemory);

  void* data;
  vkMapMemory(device_.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, indices.data(), (size_t)bufferSize);
  vkUnmapMemory(device_.device(), stagingBufferMemory);

  device_.createBuffer(
      bufferSize,
      VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
      indexBuffer_,
      indexBufferMemory_);

  device_.copyBuffer(stagingBuffer, indexBuffer_, bufferSize);
  vkDestroyBuffer(device_.device(), stagingBuffer, nullptr);
  vkFreeMemory(device_.device(), stagingBufferMemory, nullptr);
}

void LveModel::cleanup() {
  vkDestroyBuffer(device_.device(), vertexBuffer_, nullptr);
  vkFreeMemory(device_.device(), vertexBufferMemory_, nullptr);

  vkDestroyBuffer(device_.device(), indexBuffer_, nullptr);
  vkFreeMemory(device_.device(), indexBufferMemory_, nullptr);
}

std::unique_ptr<LveModel> LveModel::loadModelFromFile(LveDevice& deviceRef, std::string filepath) {
  Builder builder{};
  builder.loadModel(filepath);
  std::unique_ptr<LveModel> model = std::make_unique<LveModel>(deviceRef, builder);
  return model;
}

void LveModel::draw(VkCommandBuffer commandBuffer) {
  vkCmdDrawIndexed(commandBuffer, indexCount_, 1, 0, 0, 0);
}

void LveModel::bind(VkCommandBuffer commandBuffer) {
  VkBuffer buffers[] = {vertexBuffer_};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
  vkCmdBindIndexBuffer(commandBuffer, indexBuffer_, 0, VK_INDEX_TYPE_UINT32);
}

std::vector<VkVertexInputAttributeDescription> LveModel::Vertex::getAttributeDescriptions(
    const std::vector<VertexAttribute>& attributes) {
  std::vector<VkVertexInputAttributeDescription> attributeDescriptions(
      attributes.size(),
      VkVertexInputAttributeDescription{});
  for (int i = 0; i < attributes.size(); i++) {
    const auto& attribute = attributes[i];
    attributeDescriptions[i].binding = 0;
    attributeDescriptions[i].location = i;
    switch (attribute) {
      case VertexAttribute::POSITION:
        attributeDescriptions[i].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[i].offset = offsetof(LveModel::Vertex, position);
        break;
      case VertexAttribute::NORMAL:
        attributeDescriptions[i].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[i].offset = offsetof(LveModel::Vertex, normal);
        break;
      case VertexAttribute::COLOR:
        attributeDescriptions[i].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        attributeDescriptions[i].offset = offsetof(LveModel::Vertex, color);
        break;
      case VertexAttribute::UV:
        attributeDescriptions[i].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[i].offset = offsetof(LveModel::Vertex, uv);
        break;
      case VertexAttribute::TANGENT:
        attributeDescriptions[i].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        attributeDescriptions[i].offset = offsetof(LveModel::Vertex, tangent);
        break;
    }
  }
  return attributeDescriptions;
}

void LveModel::Builder::loadModel(std::string filepath) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string warn, err;

  if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
    throw std::runtime_error(warn + err);
  }

  vertices.clear();
  indices.clear();
  std::unordered_map<Vertex, uint32_t> uniqueVertices{};
  for (const auto& shape : shapes) {
    int count = 0;
    for (const auto& index : shape.mesh.indices) {
      Vertex vertex{};
      vertex.position = {
          attrib.vertices[3 * index.vertex_index + 0],
          attrib.vertices[3 * index.vertex_index + 1],
          attrib.vertices[3 * index.vertex_index + 2]};

      vertex.uv = {
          attrib.texcoords[2 * index.texcoord_index + 0],
          1.0f - attrib.texcoords[2 * index.texcoord_index + 1]};

      vertex.color = {1.0f, 1.0f, 1.0f, 1.0f};

      vertex.normal = {
          attrib.normals[3 * index.normal_index + 0],
          attrib.normals[3 * index.normal_index + 1],
          attrib.normals[3 * index.normal_index + 2]};

      if (uniqueVertices.count(vertex) == 0) {
        uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
        vertices.push_back(vertex);
      }
      indices.push_back(uniqueVertices[vertex]);
    }
  }
}
}  // namespace lve