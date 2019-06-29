#include "sphere.h"

using namespace glm;

Sphere::Sphere(std::string name,
  vec3 pos,
  vec3 scale,
  vec3 col,
  float ka,
  float kd,
  float ks,
  float kr,
  float n
)
{
  this->name = name;
  coefficients.ka = ka;
  coefficients.kd = kd;
  coefficients.ks = ks;
  coefficients.kr = kr;
  coefficients.n = n;

  sphereColor = col;

  transform = mat4(
    vec4(scale[0], 0.0f, 0.0f, 0.0f),
    vec4(0.0f, scale[1], 0.0f, 0.0f),
    vec4(0.0f, 0.0f, scale[2], 0.0f),
    vec4(pos, 1.0f));
  invTransform = inverse(transform);
  invNormalsTransform = transpose(invTransform);
}

std::tuple<bool, std::vector<float>> Sphere::closestIntersection(Ray r)
{
  // transformed direction in sphere space
  vec3 c1 = vec3(invTransform * vec4(r.getDirectionVector(), 0.0f));
  // transformed point in sphere space
  vec3 s1 = vec3(invTransform * vec4(r.getOriginVector(), 1.0f));

  float d = pow(dot(s1, c1), 2.0f) - dot(c1, c1) * (dot(s1, s1) - 1.0f);
  if (d < 0.0f)
    return std::make_tuple(false, std::vector<float> {0.0f});

  float t0 = (-dot(s1, c1) + sqrt(d)) / dot(c1, c1);
  float t1 = (-dot(s1, c1) - sqrt(d)) / dot(c1, c1);


  if (t0 > t1)
  {
    float swap = t0;
    t0 = t1;
    t1 = swap;
  }
  std::vector<float> intersectionsT = {t0,t1};
/* 
  if(t0>0.0001f || t1>0.0001f) return std::make_tuple(true, intersectionsT);
  else return std::make_tuple(false, intersectionsT);
  */
  return std::make_tuple(true, intersectionsT);
}

vec3 Sphere::computeNormal(vec3 point)
{
  vec3 invNormal = normalize(vec3(invTransform * vec4(point, 1.0f)));
  vec3 normal = normalize(vec3(invNormalsTransform * vec4(invNormal, 0.0f)));
  return normal;
}

vec3 Sphere::localIllum(Ray r, float t,std::vector<Sphere *> spheres, std::vector<Light *> lights, vec3 ambientColor)
{
  vec3 p = r.point(t);
  vec3 normal = computeNormal(p);
  vec3 color = sphereColor * ambientColor * coefficients.ka;

  std::vector<Light *> illumLights;
  for (unsigned int i = 0; i < lights.size(); i++)
  {
    bool isVisible = true;
    vec3 dir = lights[i]->position() - p;

    for (unsigned int k = 0; k < spheres.size(); k++)
    {
      std::tuple<bool, std::vector<float>> result = spheres[k]->closestIntersection(Ray(dir, p));
      if (std::get<bool>(result))
      {
        float t0 = std::get<std::vector<float>>(result)[0];
        float t1 = std::get<std::vector<float>>(result)[1];
        if ((t0 > 0.0001f && t0 < .9999f) ||
            (t1 > 0.0001f && t1 < .9999f))
        {
          isVisible = false;
          break;
        }
      }
    }
    if (isVisible)
      illumLights.push_back(lights[i]);
  }

  for (unsigned int i = 0; i < illumLights.size(); i++)
  {
    vec3 l = normalize(illumLights[i]->position() - p);
    // diffuse

    if (dot(normalize(r.getDirectionVector()), normal) <= 0.0f)
    {
      color += illumLights[i]->color() * coeff().kd *
               (max(dot(normal, l), 0.0f) * sphereColor);
    }
    // specular
    vec3 v = normalize(r.getDirectionVector());
    vec3 ref = normalize(reflect(l, normal));

    color += illumLights[i]->color() * coefficients.ks *
             pow(clamp(dot(ref, v), 0.0f, 1.0f), coefficients.n);
  }
  return color;
}

Coefficients Sphere::coeff()
{
  return coefficients;
}

vec3 Sphere::color()
{
  return sphereColor;
}