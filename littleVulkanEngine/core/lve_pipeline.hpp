/*
 * Little Vulkan Engine Pipeline class
 *
 * Copyright (C) 2020 by Blurrypiano - https://github.com/blurrypiano/littleVulkanEngine
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>
#include <vector>

namespace lve {

class LvePipeline {
 public:
  LvePipeline(std::string filePrefix);

 private:
  static std::vector<char> readFile(const std::string &filename);

  void createGraphicsPipeline();

  std::string filePrefix_;
};

}  // namespace lve
