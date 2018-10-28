//
// Created by wuzaiqiang on 18-10-28.
//

#ifndef DISPLAYIMAGE_RAYTRACING_H
#define DISPLAYIMAGE_RAYTRACING_H

template <typename scalar_t>
class vector3
{
public:
    scalar_t x;
    scalar_t y;
    scalar_t z;
    vector3()
    {
        x=0;
        y=0;
        z=0;
    }
    vector3 operator+(const vector3& v2)
    {
        vector3 res;
        res.x=this->x+v2.x;
        res.y=this->y+v2.y;
        res.z=this->z+v2.z;
        return res;
    }
    vector3 operator-(const vector3& v2)
    {
        vector3 res;
        res.x=this->x+v2.x;
        res.y=this->y+v2.y;
        res.z=this->z+v2.z;
        return res;
    }
    scalar_t dot(const vector3& v2)
    {
        return this->x*v2.x+this->y*v2.y+this->z*v2.z;
    }
};

#endif //DISPLAYIMAGE_RAYTRACING_H
