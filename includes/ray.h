#pragma once

#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>

#include "vec3.hpp"

class Ray {
  public:
    Ray(
      glm::vec3 c,
      glm::vec3 s
    );
    Ray(glm::vec3 dir, glm::vec3 p, int d);
    Ray(
        float near,
        float w,
        float h,
        int nCols,
        int nRows,
        int x,
        int y);

    void setDepth(int d);
    int getDepth();

    glm::vec3 point(float t);
    glm::vec3 getDirectionVector();
    glm::vec3 getOriginVector();
    
    private :
      glm::vec3 c;
      glm::vec3 s;
      int depth;
};