/*
*  Author: Davide Viero - dviero42@gmail.com
*  Rha raytracer
*  2019
*  License: see LICENSE file
* 
*/

#include <iostream>
#include <fstream>
#include <vector>

#include "sceneParser.h"
#include "sphere.h"
#include "light.h"
#include "ray.h"
#include "scene.h"

using namespace glm;

SceneParser::SceneParser(std::string filename)
{
  this->filename = filename;
}
Scene SceneParser::parse()
{
  std::fstream inputFile(filename, std::ios_base::in);

  float near = 0, left = 0, right = 0, bottom = 0, top = 0;
  float rX = 0, rY = 0;
  vec3 background;
  vec3 ambient;
  std::string outFile;
  std::string key;

  std::string name;
  float x, y, z;
  float sX, sY, sZ;
  float r, g, b;
  float ka, kd, ks, kr, n;

  std::vector<SceneObject *> objects;
  std::vector<Light *> lights;

  while (inputFile >> key)
  {
    if (key == "NEAR")
      inputFile >> near;
    else if (key == "LEFT")
      inputFile >> left;
    else if (key == "RIGHT")
      inputFile >> right;
    else if (key == "BOTTOM")
      inputFile >> bottom;
    else if (key == "TOP")
      inputFile >> top;
    else if (key == "RES")
      inputFile >> rX >> rY;
    else if (key == "SPHERE")
    {
      inputFile >> name;
      inputFile >> x >> y >> z;
      inputFile >> sX >> sY >> sZ;
      inputFile >> r >> g >> b;
      inputFile >> ka >> kd >> ks >> kr >> n;

      vec3 pos(x, y, z);
      vec3 scale(sX, sY, sZ);
      vec3 color(r, g, b);
      objects.push_back(new Sphere(name, pos, scale, color, ka, kd, ks, kr, n));
    }
    else if (key == "LIGHT")
    {
      inputFile >> name;
      inputFile >> x >> y >> z;
      inputFile >> r >> g >> b;

      vec3 pos(x, y, z);
      vec3 intensities(r, g, b);

      lights.push_back(new Light(name, pos, intensities));
    }
    else if (key == "BACK")
    {
      inputFile >> r >> g >> b;
      background = vec3(r, g, b);
    }
    else if (key == "AMBIENT")
    {
      inputFile >> r >> g >> b;
      ambient = vec3(r, g, b);
    }
    else if (key == "OUTPUT")
    {
      inputFile >> outFile;
    }
  }
  return Scene(near, left, right, bottom, top, background, ambient, objects, lights, rX,rY, outFile);
}

