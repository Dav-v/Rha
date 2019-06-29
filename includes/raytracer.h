/*
*  Author: Davide Viero - dviero42@gmail.com
*  Rha raytracer
*  2019
*  License: see LICENSE file
* 
*/

#pragma once

#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>

#include "scene.h"
#include "sceneObject.h"
#include "light.h"
#include "ray.h"

class Raytracer
{
public:
  Raytracer(Scene scene);
  glm::vec3 trace(Ray r);
  std::tuple<bool, SceneObject *, float> intersections(Ray r);

private:
  Scene scene;
};