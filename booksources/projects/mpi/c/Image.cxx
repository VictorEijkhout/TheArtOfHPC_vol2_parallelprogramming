//
// Image.cxx
//

#include "Image.h"

#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

void
Image::Write()
{
    string ext;
    switch (format)
    {
    case PPM:
        ext = ".ppm"; break;
    default:
        cerr << "ERROR: unknown image format '" << format << "'" << endl;
        return;
    }

    stringstream header;
    header << "P6" << endl;
    header << width << " " << height << endl;
    header << "255" << endl;

    fstream file;
    file.open( (filename + ext).c_str(), fstream::out | fstream::trunc | fstream::binary );
    file << header.str();
    
    // reverse row order so image is correctly oriented
    for (int j=height-1; j >= 0; --j)
    {
        int offset = j*width;
        for (int i=0; i < width; ++i)
        {
	  int pixel = offset+i;
	  int index = 3*pixel;
	  file << rgb[index+0] << rgb[index+1] << rgb[index+2];
        }
    }

    file.close();
}
