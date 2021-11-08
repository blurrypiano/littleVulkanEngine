include .env

CFLAGS = -std=c++17 -I. -I$(VULKAN_SDK_PATH)/include -I$(TINYOBJ_PATH) -g
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan

ODIR=build
DEPDIR := build
DEPFLAGS = -o $@ -MMD -MP -MF $(DEPDIR)/$*.d
SRCS = $(shell find . -type f -name "*.cpp")
OBJ_FILES = $(patsubst %.cpp, $(ODIR)/%.o, $(SRCS))

# create list of all spv files and set as dependency
vertSources = $(shell find ./shaders -type f -name "*.vert")
vertObjFiles = $(patsubst %.vert, %.vert.spv, $(vertSources))
fragSources = $(shell find ./shaders -type f -name "*.frag")
fragObjFiles = $(patsubst %.frag, %.frag.spv, $(fragSources))

TARGET = a.out
$(TARGET): $(vertObjFiles) $(fragObjFiles)
$(TARGET): $(OBJ_FILES)
	g++ $(CFLAGS) -o $(TARGET) $(OBJ_FILES) $(LDFLAGS)

$(DEPDIR): ; @mkdir -p $@
DEPFILES := $(SRCS:%.cpp=$(DEPDIR)/%.d)

$(ODIR)/%.o : %.cpp $(DEPDIR)/%.d | $(DEPDIR)
	g++ $(DEPFLAGS) $(CFLAGS) -c $<

$(DEPFILES):
include $(wildcard $(DEPFILES))

# make shader targets
%.spv: %
	${GLSLC} $< -o $@

.PHONY: test clean

test: ${TARGET}
	./${TARGET}

clean:
	rm -f ${TARGET}
	find . -name '*.spv' -delete
	find build -name '*.o' -delete
	find build -name '*.d' -delete
