/*
*  Author: Davide Viero - dviero42@gmail.com
*  Rha raytracer
*  2019
*  License: see LICENSE file
* 
*/

#include "scene.h"

Scene::Scene(
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
    )
{
  this->near = near;
  this->left = left;
  this->right = right;
  this->bottom = bottom;
  this->top = top;
  this->background = background;
  this->ambient = ambient;
  this->objects = objects;
  this->lights = lights;
  this->rX = rX;
  this->rY = rY;
  this->outputFile = outputFile;
}


float Scene::getNear()
{
  return near;
}
float Scene::getLeft()
{
  return left;
}
float Scene::getRight()
{
  return right;
}
float Scene::getBottom()
{
  return bottom;
}
float Scene::getTop()
{
  return top;
}
glm::vec3 Scene::getBackground()
{
  return background;
}
glm::vec3 Scene::getAmbient()
{
  return ambient;
}
std::vector<SceneObject *> Scene::getObjects()
{
  return objects;
}
std::vector<Light *> Scene::getLights()
{
  return lights;
}
unsigned int Scene::getRX()
{
  return rX;
}
unsigned int Scene::getRY()
{
  return rY;
}
std::string Scene::getOutFile()
{
  return outputFile;
}