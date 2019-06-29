/*
 *
 *                                                                               
*                        ..MMMMMMMMMMMN..                                 
*                    ZMMMMMMMMMMMMMMMMMMMMMMD.                            
*              ..MMMMMMMMMMMM.......NMMMMMMMMMMMM..                       
*          .+MMMMMMMMMI...    ... ..    .. ?MNMMMMMMMM .                  
*  .MMMMMMMMMMMMMD...  .,NMMMMMMMMMMMMMMM...  ...MMMMMMMMMMMMM            
*   $MMMMM=...     .MMMMMMMMMMMMMMMMMMMMMMMMMMM.   ....,MMMMMM            
*             .$MMMMMMMMMMMMMMMMMMMMM,....8MMMMMMMM..                     
*   .M7MMMMMMMMMMMM.. ..MMMMMMMMMMMMMM       ..$MMMMMMMM...               
*  .7MMMMMMMMM,.       .MMMMMMMMMMMMMM            ..MMMMMMMMMM            
*    ....OMMMMMMM..     MMMMMMMMMMMMMM              ..MMMMMMMM            
*            .MMMMMM,.   MMMMMMMMMMMM           ..MMMMMMM,..MM            
*               DMMMMMMM~..IMMMMMMM        ..MMMMMMMM.                    
*               .MMMMMMMMMMMMMM$....:NMMMMMMMMMMM..                       
*               .MMM.,MMMMMMMMMMMMMMMMMMMMMMM.                            
*               .MMM. .NMMM.  . . ...  ..                                 
*        D......MMMM.    MMMN                                             
*         ZMMMMMMMMM.     MMMM                                            
*          .MMMMMMMM.     ..MMM8                     .....                
*           .MMMMMMM.        MMMM.                .MMMMMMMM               
*            .MMMMMM.         MMMM:             .MMMMMMMMMMM.             
*             .MMMMM.          .MMMM            .MMMMMM. ?MMM             
*               MMMM.            MMMMZ          NMMMMMMM  MMM.            
*               ,MMM.             .MMMM.        .MMMMMMM  MMM             
*                ~MM.               =MMMM..      ZMMMMM  .MMM             
*                 MM.                 8MMMMM..      .,  :MMM.             
*                 .M.                   =MMMMMMM......MMMMM,              
*                  .                      .+MMMMMMMMMMMMM$                
*                                               ..NN~..         H A          
*                                                                 
*  Author: Davide Viero - dviero42@gmail.com
*  Rha raytracer
*  2019
*  License: see LICENSE file
* 
*/

#include <cstdlib>
#include <cstring>
#include <thread>
#include <iostream>
#include <fstream>
#include <vector>
#include <mutex>
#include <string>
#include "sceneObject.h"
#include "sceneParser.h"
#include "light.h"
#include "ray.h"
#include "raytracer.h"

using namespace glm;

void save_imageP3(int Width, int Height, char *fname, unsigned char *pixels);

void save_imageP6(int Width, int Height, char *fname, unsigned char *pixels);

int main(int argc, char *argv[])
{
  SceneParser parser(argv[1]);
  Scene scene = parser.parse();
 
  std::cout << "Loaded parameters:\n"
    << scene.getObjects().size() << " spheres\n"
    << scene.getLights().size() << " lights\n";
  Raytracer rha(scene);
  
  std::cout << "Raytracer started with:\n"
            << "AMBIENT: " << scene.getAmbient()[0] << "\t" << scene.getAmbient()[1] << "\t" << scene.getAmbient()[2]<<std::endl
            << "BACKGROUND: " << scene.getBackground()[0] << "\t" << scene.getBackground()[1] << "\t" << scene.getBackground()[2]<<std::endl;

  int scaleFactor = (scene.getRY() * scene.getRX()) / 100;

  std::vector<vec3> colors;
  colors.resize(scene.getRY() * scene.getRX());

  std::vector<std::thread> threads;
  unsigned int num_threads = std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() : 1;
  num_threads = 2;

  for (unsigned int t = 0; t < num_threads; t++)
  {
    std::cout<<"Launching thread number " <<t+1<<" of "<<num_threads<<std::endl;
    
    threads.push_back(std::thread([&] (unsigned int tId) {
      for (unsigned int i = 0; i < scene.getRY(); i++)
      {
        for (unsigned int k = 0; k < scene.getRX(); k++)
        {
          unsigned int currentElement = i * scene.getRY() + k;
          if ((i * scene.getRY() + k) % num_threads == tId)
          {
            Ray r(
                abs(scene.getNear()),
                (abs(scene.getLeft()) + abs(scene.getRight())) / 2.0f,
                (abs(scene.getTop()) + abs(scene.getBottom())) / 2.0f,
                scene.getRX(),
                scene.getRY(),
                i,
                k);
            if (currentElement % scaleFactor == 0)
              std::cout << "Shooting ray " << i * scene.getRY() + k << " , " << (float)(i * scene.getRY() + k) * 100.0f / (float)(scene.getRY() * scene.getRX()) << "%\n";
            
            vec3 color = rha.trace(r);
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

  int Width = scene.getRX();
  int Height = scene.getRY();
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
  strcpy(nameF, (scene.getOutFile()).c_str());
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
