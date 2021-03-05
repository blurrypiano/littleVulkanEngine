#pragma once

// std
#include <cassert>
#include <unordered_map>

// lib
#include <glm/glm.hpp>

namespace lve {

using gameObjectId_t = int64_t;

/**
 * This relates to the game object model, We're using a property-centric/pure component model. The
 * unordered map uses a fixed sized memory block (small memory allocator approach) rather than
 * something like memory pools per object type since we want to reduce fragmentation and might not
 * be able to estimate how many objects of each type we may have.
 */
template <class T, std::size_t BlockSizeBytes>
class LveComponentTable {
  static_assert(
      sizeof(T) <= BlockSizeBytes, "Size of component type for component table exceeds block size");

 public:
  LveComponentTable(std::unordered_map<gameObjectId_t, char[BlockSizeBytes]> &tableRef)
      : table{tableRef} {}

  T &operator[](int index) {
    assert(index >= 0 && index < BlockSizeBytes);
    return *reinterpret_cast<T *>(&table[index]);
  }

 private:
  std::unordered_map<gameObjectId_t, char[BlockSizeBytes]> &table;
};

// This is a component, so it will be serializable in theory
class Transform {
 public:
  glm::vec3 position;
  glm::vec4 orientation;
  glm::vec3 scale;
};

}  // namespace lve