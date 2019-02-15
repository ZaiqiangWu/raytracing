//
// Created by wuzaiqiang on 19-2-15.
//

#ifndef DISPLAYIMAGE_TRIANGLE_H
#define DISPLAYIMAGE_TRIANGLE_H

#include "vector3.h"
#include "raytracing.h"
template <typename scalar_t>
class Triangle
{
public:
    vector3<scalar_t>* p0;
    vector3<scalar_t>* p1;
    vector3<scalar_t>* p2;
    vector3<scalar_t> normal;
    Triangle()
    {
        p0=NULL;
        p1=NULL;
        p2=NULL;
        //normal=(p1-p0).cross(p2-p1).normalize();
    }
    void set_vertices(vector3<scalar_t> *point0,vector3<scalar_t> *point1,vector3<scalar_t> *point2)
    {
        p0=point0;
        p1=point1;
        p2=point2;
        normal=(*p1-*p0).cross(*p2-*p1).normalize();
    }
    bool ishit(vector3<scalar_t> e,
               vector3<scalar_t> d,
               scalar_t t0,
               scalar_t t1,
               IntersectionResult<scalar_t>& rec)
    {
        Ray<scalar_t> ray(e,d);
        scalar_t D;
        vector3<scalar_t> central=(*p0+*p1+*p2)/3;
        D=-normal.x*central.x-normal.y*central.y-normal.z*central.z;
        scalar_t t_intersect;
        t_intersect=-(normal.x*e.x+normal.y*e.y+normal.z*e.z+D)/(normal.x*d.x+normal.y*d.y+normal.z*d.z);
        if(t_intersect<t0+(scalar_t)eps||t_intersect>t1-(scalar_t)eps)
        {
            return false;
        }
        vector3<scalar_t> point;
        point=ray.getPoint(t_intersect);
        vector3<scalar_t> v0,v1,v2;
        v0=*p0-point;
        v1=*p1-point;
        v2=*p2-point;
        bool flag=true;
        if(v0.cross(v1).dot(normal)<0-eps)
            flag=false;
        if(v1.cross(v2).dot(normal)<0-eps)
            flag=false;
        if(v2.cross(v0).dot(normal)<0-eps)
            flag=false;
        if(!flag)
            return flag;
        rec.normal=normal.normalize();
        rec.position=point;
        rec.t=t_intersect;
        return flag;
    }
};
#endif //DISPLAYIMAGE_TRIANGLE_H
