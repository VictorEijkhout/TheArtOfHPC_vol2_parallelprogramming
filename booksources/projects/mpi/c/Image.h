//
// Image.h
//


#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "Color.h"

#include <string>
using namespace std;

class Image
{
 public:
    enum ImageFormat
    {
        PPM
    };

    Image(int w, int h, string fn="mpitrace", ImageFormat f=PPM)
        : width(w), height(h), filename(fn), format(f)
    {
        int size = 3*width*height;
        rgb = new unsigned char[size];
        for (int i=0; i<size; ++i)
            rgb[i] = 0;
    }

    void Add(int pixel, float* vals)
    { 
        int index=3*pixel; 
	unsigned char
	  v1 = (unsigned char)(vals[0]*256.f),
	  v2 = (unsigned char)(vals[1]*256.f),
	  v3 = (unsigned char)(vals[2]*256.f); 
        rgb[index+0] = v1;
        rgb[index+1] = v2;
        rgb[index+2] = v3;
    } 

    void Add(int pixel, ColorAccumulator& ca)
    { 
        int index=3*pixel; 
        rgb[index+0] = (unsigned char)(ca.rgba[0]/ca.rgba[3]*256.f); 
        rgb[index+1] = (unsigned char)(ca.rgba[1]/ca.rgba[3]*256.f); 
        rgb[index+2] = (unsigned char)(ca.rgba[2]/ca.rgba[3]*256.f); 
    } 

    unsigned char* GetBuffer()
    { return rgb; }

    void Write();

    ~Image()
    {
        delete[] rgb;
    }

 public:
    int width, height;
 private:    
    string filename;
    ImageFormat format;
    unsigned char* rgb;
};


#endif // _IMAGE_H_
