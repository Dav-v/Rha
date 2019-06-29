
#CXXFLAGS=-Wall -pthread -std=c++14 -g
CXXFLAGS=-Wall -pthread -std=c++14 -O2
INCLUDES=-I./includes -I./glm/glm

# list source code files for server executable
RAYTRACER_SRCS=src/main.cpp src/Light.cpp  src/Material.cpp  src/Ray.cpp  src/Raytracer.cpp  src/Scene.cpp src/SceneObject.cpp src/Material.cpp src/SceneParser.cpp src/Sphere.cpp


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
