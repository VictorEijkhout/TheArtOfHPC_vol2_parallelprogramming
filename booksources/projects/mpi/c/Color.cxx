//
//  Color.C
//

#include "Color.h"

const float ColorAccumulator::ALPHA_MAX = 0.999;

ostream& operator<<(ostream &out, ColorAccumulator const& c) 
{
    out << "(t:" << c.t << ") r:" << c.rgba[0] << " g:" << c.rgba[1] << " b:" << c.rgba[2] << " a:" << c.rgba[3];
    return out;
}
