CXX = g++
#CXX = clang++

EXE = quicknii
IMGUI_DIR = ./include/imgui
IMPLOT_DIR = ./include/implot
NIFTI_DIR = ./include/nifti_clib
BUILD_DIR = ./build
$(shell mkdir -p $(BUILD_DIR))
SRC_DIR=./src

SOURCES = $(SRC_DIR)/main.cpp
SOURCES += $(SRC_DIR)/slider.cpp
SOURCES += $(SRC_DIR)/nifti_util.cpp
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
SOURCES += $(IMPLOT_DIR)/implot.cpp $(IMPLOT_DIR)/implot_items.cpp
OBJS = $(addprefix $(BUILD_DIR)/, $(notdir $(addsuffix .o, $(basename $(SOURCES)))))

CXXFLAGS = -std=c++11 -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -I$(IMPLOT_DIR)  -I$(NIFTI_DIR) -I./include 
CXXFLAGS += -g -Wall -Wformat
LIBS =

LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
LIBS += -L/usr/local/lib -L/opt/homebrew/lib -L./lib
#LIBS += -lglfw3
LIBS += -lglfw

CXXFLAGS += -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include
CFLAGS = $(CXXFLAGS)

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

$(BUILD_DIR)/%.o:$(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o:$(IMPLOT_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o:$(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	rm -f imgui.ini
	@echo Build complete

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE)
	rm -rf $(BUILD_DIR)
	rm -f imgui.ini

clean_ini:
	rm -f imgui.ini