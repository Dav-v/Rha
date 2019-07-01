/*
*  Author: Davide Viero - dviero42@gmail.com
*  Rha raytracer
*  2019
*  License: see LICENSE file
* 
*/

#pragma once

#include <string>
#include "scene.h"

class SceneParser
{
  public:
    SceneParser(std::string filename);
    Scene parse();
  private:
    std::string filename;
};