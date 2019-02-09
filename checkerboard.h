//
// Created by wuzaiqiang on 19-2-9.
//

#ifndef DISPLAYIMAGE_CHECKERBOARD_H
#define DISPLAYIMAGE_CHECKERBOARD_H
#include "raytracing.h"
//#include "vector3.h"
template <typename scalar_t>
class checkerboard:public rectangle<scalar_t>
{
public:
    explicit checkerboard(scalar_t h=1.0, scalar_t w=1.0)
    {
        this->central=vector3<scalar_t>(0,0,0);
        this->height=h;
        this->width=w;
        this->normal=vector3<scalar_t>(0,0,1);
        this->up=vector3<scalar_t>(0,1,0);
        this->right=vector3<scalar_t>(1,0,0);
        this->p0=this->central+(scalar_t)0.5*this->height*this->up-(scalar_t)0.5*this->width*this->right;
        this->p1=this->central-(scalar_t)0.5*this->height*this->up-(scalar_t)0.5*this->width*this->right;
        this->p2=this->central-(scalar_t)0.5*this->height*this->up+(scalar_t)0.5*this->width*this->right;
        this->p3=this->central+(scalar_t)0.5*this->height*this->up+(scalar_t)0.5*this->width*this->right;
    }
    bool hit(vector3<scalar_t> e,
                     vector3<scalar_t> d,
                     scalar_t t0,
                     scalar_t t1,
                     IntersectionResult<scalar_t>& rec)
    {
        Ray<scalar_t> ray(e,d);
        scalar_t D;
        D=-this->normal.x*this->central.x-this->normal.y*this->central.y-this->normal.z*this->central.z;
        scalar_t t_intersect;
        t_intersect=-(this->normal.x*e.x+this->normal.y*e.y+this->normal.z*e.z+D)/(this->normal.x*d.x+this->normal.y*d.y+this->normal.z*d.z);
        if(t_intersect<t0+(scalar_t)eps||t_intersect>t1-(scalar_t)eps)
        {
            return false;
        }
        vector3<scalar_t> point;
        point=ray.getPoint(t_intersect);
        vector3<scalar_t> v0,v1,v2,v3;
        v0=this->p0-point;
        v1=this->p1-point;
        v2=this->p2-point;
        v3=this->p3-point;
        bool flag=true;
        if(v3.cross(v0).dot(this->normal)<0)
            flag=false;
        if(v0.cross(v1).dot(this->normal)<0)
            flag=false;
        if(v1.cross(v2).dot(this->normal)<0)
            flag=false;
        if(v2.cross(v3).dot(this->normal)<0)
            flag=false;
        if(!flag)
            return flag;
        rec.normal=this->normal.normalize();
        rec.position=point;
        rec.mtl=0;
        rec.t=t_intersect;
        scalar_t u,v;
        get_UV(point,u,v);
        if(((int)(u*10)%2==0)^((int)(v*10)%2==0))
        {
            rec.texture_color=vector3<scalar_t>(1,1,1);
        }
        else
        {
            rec.texture_color=vector3<scalar_t>(0,0,0);
        }
        return flag;
    }
    void get_UV(vector3<scalar_t> p,scalar_t &U,scalar_t &V)
    {
        //up left corner as origin
        vector3<scalar_t> pvector=p-this->p0;
        U=pvector.dot((this->p1-this->p0).normalize())/(this->p1-this->p0).length();
        V=pvector.dot((this->p3-this->p0).normalize())/(this->p3-this->p0).length();
    }
};
#endif //DISPLAYIMAGE_CHECKERBOARD_H
