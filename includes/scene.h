
/*
*  Author: Davide Viero - dviero42@gmail.com
*  Rha raytracer
*  2019
*  License: see LICENSE file
* 
*/

#pragma once

#include <vector>
#include <string>
#include "sceneObject.h"
#include "light.h"

class Scene
{
  public:
    Scene(){};
    Scene(
      float near,
      float left,
      float right,
      float bottom,
      float top,
      glm::vec3 background,
      glm::vec3 ambient,
      std::vector<SceneObject *> objects,
      std::vector<Light *> lights,
      unsigned int rX,
      unsigned int rY,
      std::string outputFile
    );
    float getNear();
    float getLeft(); 
    float getRight(); 
    float getBottom(); 
    float getTop();
    glm::vec3 getBackground();
    glm::vec3 getAmbient();
    std::vector<SceneObject *> getObjects();
    std::vector<Light *> getLights();
    unsigned int getRX();
    unsigned int getRY();
    std::string getOutFile();
  private :
    std::vector<SceneObject *> objects;
    std::vector<Light*> lights;
    float near = 0, left = 0, right = 0, bottom = 0, top = 0;
    unsigned int rX = 0, rY = 0;
    glm::vec3 background;
    glm::vec3 ambient;
    std::string outputFile;
};