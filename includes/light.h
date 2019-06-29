#pragma once

#include <cstdlib>
#include <iostream>
#include <string>
#include "vec3.hpp"

class Light {
  public:
    Light(std::string name,
           glm::vec3 position,
           glm::vec3 color
    );
    glm::vec3 position();
    glm::vec3 color();
    private : 
    std::string lightName;
    glm::vec3 lightPosition;
    glm::vec3 lightColor;
};