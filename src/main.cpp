#include <cstdlib>
#include <cstring>
#include <thread>
#include <iostream>
#include <fstream>
#include <vector>
#include <mutex>
#include <string>
#include "sphere.h"
#include "light.h"
#include "ray.h"
#include "raytracer.h"

using namespace glm;

void save_imageP3(int Width, int Height, char *fname, unsigned char *pixels);

void save_imageP6(int Width, int Height, char *fname, unsigned char *pixels);

int main(int argc, char *argv[])
{

  std::fstream inputFile(argv[1], std::ios_base::in);

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


  std::vector<Sphere *> spheres;
  std::vector<Light *> lights;

  std::vector<vec3> colors;
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
      spheres.push_back(new Sphere(name, pos, scale, color, ka, kd, ks, kr, n));
    }
    else if (key == "LIGHT")
    {
      inputFile >> name;
      inputFile >> x >> y >> z;
      inputFile >> r >> g >> b;

      vec3 pos(x, y, z);
      vec3 intensities(r,g, b);

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
  std::cout << "Loaded parameters:\n"
    << "-" << spheres.size() << " spheres\n"
    << "-" << lights.size() << " lights\n";
  Raytracer rha(ambient, background);
  std::cout << "Raytracer started with:\n"
            << "AMBIENT: " << ambient[0] << "\t" << ambient[1] << "\t" << ambient[2]<<std::endl
            << "BACKGROUND: " << background[0] << "\t" << background[1] << "\t" << background[2]<<std::endl;

  int scaleFactor = (int)floor(rY * rX) / 100;

  colors.resize(rX*rY);
  
  std::vector<std::thread> threads;
  unsigned int num_threads = std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() : 1;
  num_threads = 2;

  for (unsigned int t = 0; t < num_threads; t++)
  {
    std::cout<<"Launching thread number " <<t+1<<" of "<<num_threads<<std::endl;
    
    threads.push_back(std::thread([&] (unsigned int tId) {
      for (unsigned int i = 0; i < rY; i++)
      {
        for (unsigned int k = 0; k < rX; k++)
        {
          unsigned int currentElement = i * (int)floor(rY) + k;
          if ((i * (int)floor(rY) + k) % num_threads == tId)
          {
            Ray r(
                abs(near),
                (abs(left) + abs(right)) / 2.0f,
                (abs(top) + abs(bottom)) / 2.0f,
                rX,
                rY,
                i,
                k);
            if (currentElement % scaleFactor == 0)
              std::cout << "Shooting ray " << i * rY + k << " , " << (float)(i * rY + k) * 100.0f / (float)(rY * rX) << "%\n";
            
            vec3 color = rha.trace(r, spheres, lights);
            // No data races - every thread writes only one element every num_threads elements
            colors[currentElement] =color;
          }
        }
      }
    }, t));
  }
  for (unsigned int i = 0; i < num_threads; i++)
  {
    threads[i].join();
  }

  int Width = rX;
  int Height = rY;
  unsigned char *pixels;

  pixels = new unsigned char[3 * Width * Height];

  int k = 0;
  std::cout<<"Starting pixel conversion\n";
  threads.clear();
  /*
  for (unsigned int t = 0; t < num_threads; t++)
  {
    std::cout << "Lanuching thread number " << t+1 << " of " << num_threads << std::endl;

    threads.push_back(std::thread([&](unsigned int tId) {
  */
        for (int i = 0; i < Height; i++)
        {
          for (int j = 0; j < Width; j++)
          {
            //if ((i * (int)floor(rY) + j) % num_threads == tId)
            {
            pixels[k] = int(colors[(Height - i - 1) * Height + j][0] * 255.0f);
            pixels[k + 1] = int(colors[(Height - i - 1) * Height + j][1] * 255.0f);
            pixels[k + 2] = int(colors[(Height - i - 1) * Height + j][2] * 255.0f);
            k = k + 3;
            }
          }
        }
  /*
    },t));
  }
  for (unsigned int i = 0; i < num_threads; i++)
  {
    threads[i].join();
  }
  */
  char nameF[21];
  strcpy(nameF, (outFile).c_str());
  save_imageP3(Width, Height, nameF, pixels);
  //save_imageP6(Width, Height, nameF, pixels);

}

// Output in P6 format, a binary file containing:
// P6
// ncolumns nrows
// Max colour value
// colours in binary format thus unreadable
void save_imageP6(int Width, int Height, char *fname, unsigned char *pixels)
{
  FILE *fp;
  const int maxVal = 255;

  printf("Saving image %s: %d x %d\n", fname, Width, Height);
  fp = fopen(fname, "wb");
  if (!fp)
  {
    printf("Unable to open file '%s'\n", fname);
    return;
  }
  fprintf(fp, "P6\n");
  fprintf(fp, "%d %d\n", Width, Height);
  fprintf(fp, "%d\n", maxVal);

  for (int j = 0; j < Height; j++)
  {
    fwrite(&pixels[j * Width * 3], 3, Width, fp);
  }

  fclose(fp);
}

void save_imageP3(int Width, int Height, char *fname, unsigned char *pixels)
{
  FILE *fp;
  const int maxVal = 255;

  printf("Saving image %s: %d x %d\n", fname, Width, Height);
  fp = fopen(fname, "w");
  if (!fp)
  {
    printf("Unable to open file '%s'\n", fname);
    return;
  }
  fprintf(fp, "P3\n");
  fprintf(fp, "%d %d\n", Width, Height);
  fprintf(fp, "%d\n", maxVal);

  int k = 0;
  for (int j = 0; j < Height; j++)
  {

    for (int i = 0; i < Width; i++)
    {
      fprintf(fp, " %d %d %d", pixels[k], pixels[k + 1], pixels[k + 2]);
      k = k + 3;
    }
    fprintf(fp, "\n");
  }
  fclose(fp);
}
