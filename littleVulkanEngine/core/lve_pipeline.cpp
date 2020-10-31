/*
 * Little Vulkan Engine Pipeline class
 *
 * Copyright (C) 2020 by Blurrypiano - https://github.com/blurrypiano/littleVulkanEngine
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "lve_pipeline.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace lve {

LvePipeline::LvePipeline(std::string filePrefix) : filePrefix_{filePrefix} {
  createGraphicsPipeline();
}

std::vector<char> LvePipeline::readFile(const std::string &filename) {
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    throw std::runtime_error("failed to open file!");
  }

  size_t fileSize = (size_t)file.tellg();
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();

  return buffer;
}

void LvePipeline::createGraphicsPipeline() {
  auto vertCode = readFile(filePrefix_ + ".vert.spv");
  auto fragCode = readFile(filePrefix_ + ".frag.spv");

  std::cout << "vertShaderCode size: " << vertCode.size() << '\n';
  std::cout << "fragShaderCode size: " << fragCode.size() << '\n';
}

}  // namespace lve