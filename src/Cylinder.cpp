/*
*  Author: Davide Viero - dviero42@gmail.com
*  Rha raytracer
*  2019
*  License: see LICENSE file
* 
*/

#define GLM_ENABLE_EXPERIMENTAL
#include "cylinder.h"
#include "gtx/transform.hpp"
#include <iostream>
using namespace glm;

Cylinder::Cylinder(std::string name,
               vec3 pos,
               vec3 scale,
               vec3 col,
               float ka,
               float kd,
               float ks,
               float kr,
               float n)
{
  this->name = name;
  coefficients.ka = ka;
  coefficients.kd = kd;
  coefficients.ks = ks;
  coefficients.kr = kr;
  coefficients.n = n;

  color = col;

  //transform = mat4(1.0f);
  transform = translate(transform,pos);
  transform = glm::scale(transform,scale);
  transform = rotate(transform, 90.0f, vec3(1.0, 0.0f,0.0f));

  for(int i = 0; i < 4; i++){
    std::cout<<"\n";
    for(int k = 0; k < 4; k++)
    std::cout<<transform[k][i]<<"\t";
  }  

  normalsTransform = transpose(transform);
  invTransform = inverse(transform);
  invNormalsTransform = transpose(invTransform);
}

vec3 Cylinder::computeNormal(vec3 point)
{
  vec3 invNormal;
      vec3 localSpacePoint = vec3(invTransform * vec4(point, 1.0f));
  if (localSpacePoint.z > 0.49 && localSpacePoint.z < 0.51)
    invNormal = vec3(0.0f,0.0f,1.0f);
  else if (localSpacePoint.z > -0.51 && localSpacePoint.z < -0.49)
    invNormal = vec3(0.0f, 0.0f, -1.0f);
  else
    invNormal = normalize(vec3(localSpacePoint.x,localSpacePoint.y, 0.0f));
  vec3 normal = normalize(vec3(invNormalsTransform * vec4(invNormal, 0.0f)));
  return normal;
}


std::tuple<bool, std::vector<float>> Cylinder::intersections(Ray r)
{
  // transformed direction in cylinder space
  vec3 c1 = vec3(invTransform * vec4(r.getDirectionVector(), 0.0f));
  // transformed point in cylinder space
  vec3 s1 = vec3(invTransform * vec4(r.getOriginVector(), 1.0f));

  /*
  Equation for a ray intersection a cylinder : 
  t^2 (dot(cd,cd)) + 2*t (dot(cd,sd)) + (dot(sd,sd) -1) = 0
  where cd = vec2(c1.x,c1.y) sd = vec2(s1.x,s1.y)
  z must be -0.5<= z <= 0.5
   */
  vec2 cd = vec2(c1);
  vec2 sd = vec2(s1);
  float a = dot(cd, cd);
  float b = dot(sd, cd);
  float c = (dot(sd, sd) - 1.0f);
  
  float d = pow(b, 2.0f) - a * c;
  if (d < 0.0f)
    return std::make_tuple(false, std::vector<float>{0.0f});

  float t0 = (-b + sqrt(d)) / a;
  float t1 = (-b - sqrt(d)) / a;

  bool intersected = false;
  vec3 p0 = c1 * t0 +s1;
  vec3 p1 = c1 * t1 +s1;

  std::vector<float> intersectionsT;
  if(p0.z >= -0.5 && p0.z <= 0.5)
  {
    intersected = true;
    intersectionsT.push_back(t0);
  }
  if (p1.z >= -0.5 && p1.z <= 0.5)
  {
    intersected = true;
    intersectionsT.push_back(t1);
  }
  float t2 = (0.5f-s1.z) / c1.z;
  float t3 = (-0.5f-s1.z) / c1.z;

  float x = c1.x * t2 + s1.x;
  float y = c1.y * t2 + s1.y;
  if(pow(x,2)+pow(y,2) <= 1.0f)
  {
    intersected = true;
    intersectionsT.push_back(t2);
  }
  x = c1.x * t3 + s1.x;
  y = c1.y * t3 + s1.y;
  if(pow(x,2)+pow(y,2) <= 1.0f)
  {
    intersected = true;
    intersectionsT.push_back(t3);
  }

  /* 
  if(t0>0.0001f || t1>0.0001f) return std::make_tuple(true, intersectionsT);
  else return std::make_tuple(false, intersectionsT);
  */
  return std::make_tuple(intersected, intersectionsT);
}