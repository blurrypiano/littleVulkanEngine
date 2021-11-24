include(./.env.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)
message(STATUS "Local .env: ${LOCAL_ENV}")

cmake_minimum_required(VERSION 3.18.4)

set(NAME LittleVulkanEngine)

project(${NAME} VERSION 0.23.0)

# configure_file(LVEConfig.h.in LVEConfig.h)

if (DEFINED VULKAN_SDK_PATH)
  set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include")
  set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib")
else()
  find_package(Vulkan REQUIRED)
  message(STATUS "Found Vulkan: $ENV{VULKAN_SDK}")
endif()

if (DEFINED GLFW_PATH)
  message(STATUS "Using GLFW path specified in .env")
  set(GLFW_INCLUDE_DIRS "${GLFW_PATH}/include")
  set(GLFW_LIB "${GLFW_PATH}/lib-vc2019")
  # set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib")
else()
  find_package(glfw3 3.3 REQUIRED)
  set(GLFW_LIB glfw)
  message(STATUS "Found GLFW")
endif()

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

if (WIN32)
  message(STATUS "CREATING BUILD FOR WINDOWS")

  target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${Vulkan_INCLUDE_DIRS}
    ${TINYOBJ_PATH}
    ${GLFW_INCLUDE_DIRS}
    ${GLM_PATH}
    )

  # target_include_directories(${PROJECT_NAME} PUBLIC
  #     $ENV{MINGW_ROOT}/x86_64-w64-mingw32/include
  #     $ENV{USERPROFILE}/dev/sdks/vulkan/Include
  #     $ENV{USERPROFILE}/dev/sdks/glfw/include
  #     $ENV{USERPROFILE}/dev/sdks/glm/glm
  #     ${PROJECT_SOURCE_DIR}/src/third_party
  # )

  target_link_directories(${PROJECT_NAME} PUBLIC
    #   $ENV{MINGW_ROOT}/x86_64-w64-mingw32/lib
    #   $ENV{USERPROFILE}/dev/sdks/glfw/lib-mingw-w64
    ${Vulkan_LIBRARIES}
    ${GLFW_LIB}
  )

    target_link_libraries(${PROJECT_NAME} glfw3 vulkan-1)
elseif (UNIX)
    message(STATUS "CREATING BUILD FOR UNIX")
    target_include_directories(${PROJECT_NAME} PUBLIC  ${TINYOBJ_PATH})
    target_link_libraries(${PROJECT_NAME} glfw vulkan)
endif()


# target_link_libraries(main -lpthread)
# target_link_libraries(${PROJECT_NAME} -lglfw)
# target_link_libraries(${PROJECT_NAME} -lvulkan)
# target_link_libraries(main -ldl)
# target_link_libraries(main -lpthread)
# target_link_libraries(main -lX11)
# target_link_libraries(main -lXxf86vm)
# target_link_libraries(main -lXrandr)
# target_link_libraries(main -lXi)


# Find all vertex and fragment sources within shaders directory
# taken from VBlancos vulkan tutorial
# https://github.com/vblanco20-1/vulkan-guide/blob/all-chapters/CMakeLists.txt
find_program(GLSL_VALIDATOR glslangValidator HINTS /usr/bin /usr/local/bin $ENV{VULKAN_SDK}/Bin/ $ENV{VULKAN_SDK}/Bin32/)
file(GLOB_RECURSE GLSL_SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/shaders/*.frag"
    "${PROJECT_SOURCE_DIR}/shaders/*.vert"
    )

foreach(GLSL ${GLSL_SOURCE_FILES})
  message(STATUS "BUILDING SHADER")
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SPIRV "${PROJECT_SOURCE_DIR}/shaders/${FILE_NAME}.spv")
  message(STATUS ${GLSL})
  add_custom_command(
    OUTPUT ${SPIRV}
    COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
    DEPENDS ${GLSL})
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
    Shaders
    DEPENDS ${SPIRV_BINARY_FILES}
)