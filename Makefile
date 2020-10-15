include .env

CFLAGS = -std=c++17 -I$(THINENG_PATH) -I$(VULKAN_SDK_PATH)/include -I$(STB_INCLUDE_PATH) -I$(TINYOBJ_INCLUDE_PATH)
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan

ODIR=build
DEPDIR := build
DEPFLAGS = -o $@ -MMD -MP -MF $(DEPDIR)/$*.d
SRCS = $(shell find littleVulkanEngine -type f -name "*.cpp")

coreSources = $(shell find littleVulkanEngine/core -type f -name "*.cpp")
coreObjFiles = $(patsubst %.cpp, $(ODIR)/%.o, $(coreSources))

TARGET = a.out
${TARGET}: $(coreObjFiles)
	g++ $(CFLAGS) -o ${TARGET} $(coreObjFiles) $(LDFLAGS)

$(DEPDIR): ; @mkdir -p $@
DEPFILES := $(SRCS:%.cpp=$(DEPDIR)/%.d)

COMPILE.c = g++ $(DEPFLAGS) $(CFLAGS) -c

$(ODIR)/%.o : %.cpp $(DEPDIR)/%.d | $(DEPDIR)
	g++ $(DEPFLAGS) $(CFLAGS) -c $<

$(DEPFILES):
include $(wildcard $(DEPFILES))

.PHONY: test clean

test: ${TARGET}
	LD_LIBRARY_PATH=$(VULKAN_SDK_PATH)/lib VK_LAYER_PATH=$(VULKAN_SDK_PATH)/etc/vulkan/explicit_layer.d ./${TARGET}

clean:
	rm -f ${TARGET}