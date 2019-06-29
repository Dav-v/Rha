#include <tuple>
#include "material.h"
#include "ray.h"
#include "light.h"
//TODO better imports lights
#include "mat4x4.hpp"

class SceneObject{
  public:
    virtual std::tuple<bool, std::vector<float>> closestIntersection(Ray r) = 0;
    glm::mat4 getTransform() {return transform;};
    glm::mat4 getInvTransform() { return invTransform; };
    glm::mat4 getInvNormalsTransform() { return invNormalsTransform; };
  protected:
    glm::mat4 transform = glm::mat4(1.0f);
    glm::mat4 invTransform = glm::mat4(1.0f);
    glm::mat4 invNormalsTransform = glm::mat4(1.0f);
    Material material;
};