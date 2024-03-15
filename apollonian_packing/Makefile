CXX = clang++
CXX_VERSION = c++20

NAME = apollonian_packing

INCLUDE = -Iinclude/ -I../external/raylib/include 
SRC_DIR = src

INTERMEDIATE_DIR = ./out/obj/

TARGET_DIR = ../out/
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
SRC_FILES += $(wildcard $(SRC_DIR)/*/*.cpp)


TARGET = $(TARGET_DIR)$(NAME)

OBJECTS = $(patsubst %.cpp,$(INTERMEDIATE_DIR)%.o,$(SRC_FILES))
# OBJECTS += external/raylib/include/raudio.o
LIB_INCLUDE_DIRS = -L$(TARGET_DIR) -L../external/raylib/lib

LIBS = -lraylib -lGL 

DEPS = $(OBJECTS)

$(NAME): $(TARGET)

$(TARGET): $(DEPS) | $(TARGET_DIR)
	$(CXX) -o $@ -std=$(CXX_VERSION) $(OBJECTS) $(LIB_INCLUDE_DIRS) $(LIBS)

$(INTERMEDIATE_DIR)%.o: %.cpp | $(INTERMEDIATE_DIR)
	$(CXX) -std=$(CXX_VERSION) $(INCLUDE) -c $< -o $@

$(TARGET_DIR)%a:
	make -C ../$(subst .a,,$(@F))

run: $(NAME)
	$(TARGET)

$(TARGET_DIR):
	mkdir -p $(TARGET_DIR)

$(INTERMEDIATE_DIR):
	mkdir -p $(INTERMEDIATE_DIR)src
