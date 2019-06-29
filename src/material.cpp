#include "material.h"

using namespace glm;

Material::Material(
  std::string name,
  vec3 ka,
  vec3 kd,
  vec3 ks,
  vec3 kr,
  float n){
  this->name = name;
  Kambient = ka;
  Kdiffuse = kd;
  Kspecular = ks;
  Kreflection = kr;
  brightness = n;
}

/**********************
 * 
 *  DEFAULT MATERIALS
 */

Material::Material(std::string name){
  if(name == "IRON"){

  }
  else if (name == "GLASS"){
    
  }
}

