CFLAGS+= -Iinclude -Iphoton -std=c++17
# CFLAGS+= `sdl2-config --cflags`

# LDFLAGS+= `sdl2-config --libs` -lSDL2_image

RM=rm -rf

DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPS_DIR)/$*.d

SRCS_DIR = src
BUILD_DIR = build
OBJS_DIR = $(BUILD_DIR)/objs
DEPS_DIR = $(BUILD_DIR)/deps
EXEC_DIR = bin

PRECOMPILE = mkdir -p $(dir $@); mkdir -p $(dir $(DEPS_DIR)/$*); mkdir -p $(EXEC_DIR)

EXEC=$(EXEC_DIR)/coinche

include src.mk

OBJS = $(patsubst $(SRCS_DIR)/%.cpp, $(OBJS_DIR)/%.o, $(SRCS))
DEPS = $(patsubst $(SRCS_DIR)/%.cpp, $(DEPS_DIR)/%.d, $(SRCS))

all: $(EXEC)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@$(PRECOMPILE)
	g++ $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(EXEC): $(OBJS)
	g++ -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(BUILD_DIR)

fclean: clean
	$(RM) $(EXEC_DIR)

re: fclean all

include $(wildcard $(DEPS))

.PHONY: all clean mrproper