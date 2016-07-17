CXX = g++
CXXFLAGS = -g -Wall -O2

MAIN_TARGET = blues

SRC_DIR = src
OBJ_DIR = obj
DEPS_DIR = deps
INCLUDE_DIR = include
BRIDGE_DIR = bridge

SRC_MODULES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_MODULES = $(addprefix $(OBJ_DIR)/, $(notdir $(SRC_MODULES:.cpp=.o)))
DEPS_MODULES = $(addprefix $(DEPS_DIR)/, $(notdir $(SRC_MODULES:.cpp=.d)))

CXXFLAGS += -I $(INCLUDE_DIR)
CXXFLAGS += -I $(BRIDGE_DIR)/include

LDFLAGS = -L $(BRIDGE_DIR)/lib
LDFLAGS += -l argvparser

BRIDGE_TARGETS = Argvparser

src_to_obj = $(addprefix $(OBJ_DIR)/, $(notdir $(1:.cpp=.o)))

all: $(MAIN_TARGET)

ifneq ($(MAKECMDGOALS), clean)
-include bridge.touch
endif

$(MAIN_TARGET): $(OBJ_MODULES)
	$(CXX) $(CXXFLAGS) $^ -o $(MAIN_TARGET) $(LDFLAGS)

$(DEPS_DIR)/%.d: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -E -MM -MT $(call src_to_obj, $<) -MT $@ -MF $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

bridge.touch:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(DEPS_DIR)
	mkdir -p $(BRIDGE_DIR)/include
	mkdir -p $(BRIDGE_DIR)/lib
	make -C $(BRIDGE_DIR) -f Makefile $(BRIDGE_TARGETS)
	echo "-include $(DEPS_MODULES)" > $@

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(DEPS_DIR)
	rm -rf $(BRIDGE_DIR)/include
	rm -rf $(BRIDGE_DIR)/lib
	make -C $(BRIDGE_DIR) -f Makefile clean
	rm -f bridge.touch
	rm -rf $(MAIN_TARGET).dSYM
	rm -f $(MAIN_TARGET)

.PHONY: all clean
