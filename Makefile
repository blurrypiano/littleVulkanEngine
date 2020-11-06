include .env

CFLAGS = -std=c++17 -I$(THINENG_PATH) -I$(VULKAN_SDK_PATH)/include -I$(STB_INCLUDE_PATH) -I$(TINYOBJ_INCLUDE_PATH) -g
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan

ODIR=build
DEPDIR := build
DEPFLAGS = -o $@ -MMD -MP -MF $(DEPDIR)/$*.d
SRCS = $(shell find littleVulkanEngine -type f -name "*.cpp")

coreSources = $(shell find littleVulkanEngine/core -type f -name "*.cpp")
coreObjFiles = $(patsubst %.cpp, $(ODIR)/%.o, $(coreSources))

generateSources = $(shell find littleVulkanEngine/generate -type f -name "*.cpp")
generateObjFiles = $(patsubst %.cpp, $(ODIR)/%.o, $(generateSources))

vertSources = $(shell find shaders -type f -name "*.vert")
vertObjFiles = $(patsubst %.vert, %.vert.spv, $(vertSources))
fragSources = $(shell find shaders -type f -name "*.frag")
fragObjFiles = $(patsubst %.frag, %.frag.spv, $(fragSources))

TARGET = a.out
$(TARGET): $(vertObjFiles) $(fragObjFiles)
${TARGET}: $(coreObjFiles)
${TARGET}: $(generateObjFiles)
	g++ $(CFLAGS) -o ${TARGET} $(coreObjFiles) $(generateObjFiles) $(LDFLAGS)

$(DEPDIR): ; @mkdir -p $@
DEPFILES := $(SRCS:%.cpp=$(DEPDIR)/%.d)

COMPILE.c = g++ $(DEPFLAGS) $(CFLAGS) -c

$(ODIR)/%.o : %.cpp $(DEPDIR)/%.d | $(DEPDIR)
	g++ $(DEPFLAGS) $(CFLAGS) -c $<


libs: $(TARGET)
	ar rvs lvecore.a $(coreObjFiles)
	ar rvs lvegenerate.a $(generateObjFiles)

$(DEPFILES):
include $(wildcard $(DEPFILES))

# make shader targets
%.vert.spv: %.vert
	${GLSLC} $< -o $@

%.frag.spv: %.frag
	${GLSLC} $< -o $@

.PHONY: test clean

test: ${TARGET}
	LD_LIBRARY_PATH=$(VULKAN_SDK_PATH)/lib VK_LAYER_PATH=$(VULKAN_SDK_PATH)/etc/vulkan/explicit_layer.d ./${TARGET}

clean:
	rm -f ${TARGET}
	find build -name '*.o' -delete
	find build -name '*.d' -delete
