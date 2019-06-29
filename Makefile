
#CXXFLAGS=-Wall -pthread -std=c++14 -g
CXXFLAGS=-Wall -pthread -std=c++14 -O2
INCLUDES=-I./includes -I./glm/glm

# list source code files for server executable
RAYTRACER_SRCS=src/main.cpp src/light.cpp  src/material.cpp  src/ray.cpp  src/raytracer.cpp  src/sphere.cpp


RAYTRACER_OBJS=$(RAYTRACER_SRCS:.c=.o)

all: raytracer

raytracer: $(RAYTRACER_OBJS)
	@echo "Making executable: "$@
	@echo "$(CXX) $^ $(CXXFLAGS)  $(INCLUDES) -o $@"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

.cpp.o:
	@echo "Compiling: "$<
	@echo "$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean

clean:
	@rm -f src/*.o raytracer
	@echo "Removed object files and executables..."
