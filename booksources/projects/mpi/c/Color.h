//
//  Color.h
//

#ifndef _COLOR_H_
#define _COLOR_H_

#include <cfloat>
#include <iostream>
using namespace std;

// parent class for CHat, CTilde
class ColorAccumulator {
 public:
    ColorAccumulator()
        { t=FLT_MAX, rgba[0] = 0.; rgba[1] = 0.; rgba[2] = 0.; rgba[3] = 0.; }
    ColorAccumulator(float t_, float *rgba_)
        { t=t_; rgba[0] = rgba_[0]; rgba[1] = rgba_[1]; rgba[2] = rgba_[2]; rgba[3] = rgba_[3]; }
    ColorAccumulator(float t_, float r, float g, float b, float a)
        { t=t_; rgba[0] = r; rgba[1] = g; rgba[2] = b; rgba[3] = a; }
    ColorAccumulator(const ColorAccumulator &c)
        { t=c.t; rgba[0] = c.rgba[0]; rgba[1] = c.rgba[1]; rgba[2] = c.rgba[2]; rgba[3] = c.rgba[3]; }        
    ColorAccumulator(const unsigned char* buf)
        { t=*((float*)(buf+0)); rgba[0]=*((float*)(buf+4)); rgba[1]=*((float*)(buf+8)); rgba[2]=*((float*)(buf+12)); rgba[3]=*((float*)(buf+16)); }

    ColorAccumulator& operator=(const ColorAccumulator&);
    void add(const ColorAccumulator&);
    virtual void accumulate(const ColorAccumulator&) = 0;
    bool operator<(const ColorAccumulator&) const;
    bool operator>(const ColorAccumulator&) const;
    int pack (unsigned char* buf);
    void unpack (const unsigned char* buf);
    void clear() { t=FLT_MAX, rgba[0] = 0.f; rgba[1] = 0.f; rgba[2] = 0.f; rgba[3] = 0.f; }
    static int packedSize() { return 20; }

    friend ostream& operator<<(ostream&, ColorAccumulator const&);

    float t;
    float rgba[4];

    const static float ALPHA_MAX;
};

// represents C-Hat, as defined in Levoy 1990
// devide rgb by a to get normalized color
class CHat : public ColorAccumulator {
 public:
    CHat() : ColorAccumulator() {}
    CHat(float t_, float *rgba_) : ColorAccumulator( t_, rgba_) {}
    CHat(float t_, float r, float g, float b, float a) : ColorAccumulator( t_,r,g,b,a ) {}
    CHat(const CHat &c) : ColorAccumulator( c ) {}
    CHat(const unsigned char* buf) : ColorAccumulator( buf ) {}
    
    void accumulate(const ColorAccumulator&);
};

// represents C-Tilde, as defined in Blinn 1994, Wittenbrink 1998
// stores associated (Blinn) or opacity-weighted (Wittenbrink) color
class CTilde : public ColorAccumulator {
 public:
    CTilde() : ColorAccumulator() {}
    CTilde(float t_, float *rgba_) : ColorAccumulator( t_, rgba_) {}
    CTilde(float t_, float r, float g, float b, float a) : ColorAccumulator( t_,r,g,b,a ) {}
    CTilde(const CTilde &c) : ColorAccumulator( c ) {}
    CTilde(const unsigned char* buf) : ColorAccumulator( buf ) {}

    void accumulate(const ColorAccumulator&);
};

inline ColorAccumulator& ColorAccumulator::operator=(const ColorAccumulator& c)
{ 
    if (this == &c) return *this;

    t = c.t;
    rgba[0] = c.rgba[0]; 
    rgba[1] = c.rgba[1]; 
    rgba[2] = c.rgba[2]; 
    rgba[3] = c.rgba[3]; 

    return *this;
}

inline bool ColorAccumulator::operator<(const ColorAccumulator& c) const
{
    return t < c.t;
}

inline bool ColorAccumulator::operator>(const ColorAccumulator& c) const
{
    return t > c.t;
}

inline void ColorAccumulator::add(const ColorAccumulator& c) 
{
    rgba[0] += c.rgba[0];
    rgba[1] += c.rgba[1];
    rgba[2] += c.rgba[2];
    // do not change opacity.  use accumulate for that.
}

inline int ColorAccumulator::pack(unsigned char* buf)
{
    *((float*)(buf+ 0)) = t;
    *((float*)(buf+ 4)) = rgba[0];
    *((float*)(buf+ 8)) = rgba[1];
    *((float*)(buf+12)) = rgba[2];
    *((float*)(buf+16)) = rgba[3];

    return 20;
}

inline void ColorAccumulator::unpack(const unsigned char* buf)
{
    t       = *((float*)(buf+ 0));
    rgba[0] = *((float*)(buf+ 4));
    rgba[1] = *((float*)(buf+ 8));
    rgba[2] = *((float*)(buf+12));
    rgba[3] = *((float*)(buf+16));
}

// composite the color c into this color, as described in levoy 1990
inline void CHat::accumulate(const ColorAccumulator& c)
{
    t = c.t; // depth value;

    double one_a = 1. - rgba[3];
    rgba[0] = rgba[0]*rgba[3] + c.rgba[0]*c.rgba[3]*one_a;
    rgba[1] = rgba[1]*rgba[3] + c.rgba[1]*c.rgba[3]*one_a;
    rgba[2] = rgba[2]*rgba[3] + c.rgba[2]*c.rgba[3]*one_a;
    rgba[3] =         rgba[3] +           c.rgba[3]*one_a;
}

// composite the color c into this color, as described in blinn 1994, wittenbrink 1998
inline void CTilde::accumulate(const ColorAccumulator& c)
{
    t = c.t; // depth value;

    double one_a = 1. - rgba[3];
    rgba[0] += c.rgba[0]*one_a;
    rgba[1] += c.rgba[1]*one_a;
    rgba[2] += c.rgba[2]*one_a;
    rgba[3] += c.rgba[3]*one_a;
}

#endif // _COLOR_H_
