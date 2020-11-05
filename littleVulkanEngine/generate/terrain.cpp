#include "terrain.hpp"

#include "perlin_noise.hpp"

namespace lve {
Terrain::Terrain(LveDevice& device) : device_{device} {
  generateTerrainModel(VERTEX_COUNT, SIZE, 1.0f);
}

void Terrain::generateTerrainModel(int vertexCount, float size, float textureScaleFactor) {
  LveModel::Builder builder{};
  assert(vertexCount > 1);

  HeightMap heightMap = generateHeightMap(vertexCount, size, 0.0f, 100.0f);

  int count = vertexCount * vertexCount;
  float delta = size / static_cast<float>(vertexCount);
  float invScale = 1.0f / textureScaleFactor;
  for (int i = 0; i < vertexCount; i++) {
    for (int j = 0; j < vertexCount; j++) {
      LveModel::Vertex vertex{};
      float height = heightMap[i][j];
      vertex.position = {j * delta, height, i * delta};
      vertex.uv = {j * invScale, i * invScale};
      vertex.normal = calculateNormal(heightMap, delta, i, j);
      vertex.tangent = calculateTangent(heightMap, i, j);
      // float hRatio = 0.5f * ((height / amplitude) + 1);
      // vertex.color = glm::mix(glm::vec3(0.92f, 0.42f, 0.18f), glm::vec3(0.5f, 1.0f, 0.43f),
      // hRatio);
      vertex.color = glm::vec4(0.5f, 1.0f, 0.43f, 1.0f);

      builder.vertices.push_back(vertex);
    }
  }

  for (int i = 0; i < vertexCount - 1; i++) {
    for (int j = 0; j < vertexCount - 1; j++) {
      builder.indices.push_back(j + i * vertexCount);
      builder.indices.push_back(j + (i + 1) * vertexCount);
      builder.indices.push_back(j + (i + 1) * vertexCount + 1);
      builder.indices.push_back(j + i * vertexCount);
      builder.indices.push_back(j + (i + 1) * vertexCount + 1);
      builder.indices.push_back(j + i * vertexCount + 1);
    }
  }

  model = std::make_unique<LveModel>(device_, builder);
}

HeightMap Terrain::generateHeightMap(
    int vertexCount, float size, float minHeight, float maxHeight) {
  assert(maxHeight >= minHeight);

  PerlinNoise generator{};
  HeightMap heightMap(vertexCount, std::vector<float>(vertexCount, 1.0f));
  float amplitude = maxHeight - minHeight;
  float scale = 1.0f / static_cast<float>(vertexCount);
  for (int i = 0; i < vertexCount; i++) {
    for (int j = 0; j < vertexCount; j++) {
      // by increasing the samples we can smooth out the noise of our generated texture?
      // would jitering help?
      float n0 = generator.perlinNoise(scale * glm::vec3(i, 0, j), 4, 2.0f, 0.8f);
      heightMap[i][j] = amplitude * 0.5f * (n0 + 1) + minHeight;
      // heightMap[i][j] = amplitude * glm::abs(n0) + minHeight;
      // heightMap[i][j] = amplitude * n0 * n0 + minHeight;
    }
  }
  return heightMap;
}

glm::vec3 Terrain::calculateNormal(const HeightMap& heightMap, float delta, int i, int k) {
  size_t vertexCount = heightMap.size();

  // if we fit a quadratic curve to our 3 heights, the derivative is (h2 - h0) / 2
  float left = i - 1 >= 0 ? heightMap[i - 1][k] : heightMap[i][k];
  float right = i + 1 < vertexCount ? heightMap[i + 1][k] : heightMap[i][k];
  float bottom = k - 1 >= 0 ? heightMap[i][k - 1] : heightMap[i][k];
  float top = k + 1 < vertexCount ? heightMap[i][k + 1] : heightMap[i][k];

  float dhdx = 0.5f * (right - left);
  float dhdz = 0.5f * (bottom - top);

  // gradient
  // auto tangent = glm::vec3(1.0f, dhdx, 0.0f);
  // auto bitangent = glm::vec3(0.0f, dhdz, 1.0f);
  // return -glm::normalize(glm::cross(bitangent, tangent));
  return glm::normalize(glm::vec3(dhdx, -delta, dhdz));
}

glm::vec4 Terrain::calculateTangent(const HeightMap& heightMap, int i, int k) {
  size_t vertexCount = heightMap.size();
  float left = i - 1 >= 0 ? heightMap[i - 1][k] : 0.0f;
  float right = i + 1 < vertexCount ? heightMap[i + 1][k] : 0.0f;
  float dhdx = 0.5f * (right - left);
  // tangent
  return glm::vec4(glm::normalize(glm::vec3(1.0f, dhdx, 0.0f)), 1.0);
}

}  // namespace lve
